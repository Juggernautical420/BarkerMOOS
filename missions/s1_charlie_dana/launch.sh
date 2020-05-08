#!/bin/bash 
#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
TIME_WARP=1
JUST_MAKE="no"
RANDSTARTIN="true"
RANDSTARTOUT="true"
ORDER="normal"
INAMT=1
OUTAMT=1
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
    printf "%s [SWITCHES] [time_warp]          \n" $0
    printf "  --just_make, -j                  \n" 
    printf "  --help, -h                       \n" 
    printf "  --in=N                          \n" 
    printf "  --out=N                          \n" 
    printf "  --norand                         \n" 
    exit 0;
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
    JUST_MAKE="yes"
    elif [ "${ARGI}" = "--randin" -o "${ARGI}" = "-ri" ] ; then
    RANDSTARTIN="true"
        elif [ "${ARGI}" = "--randout" -o "${ARGI}" = "-ro" ] ; then
    RANDSTARTOUT="true"
    elif [ "${ARGI:0:5}" = "--in=" ] ; then
        INAMT="${ARGI#--in=*}"
    elif [ "${ARGI:0:6}" = "--out=" ] ; then
        OUTAMT="${ARGI#--out=*}"    
    else 
    printf "Bad Argument: %s \n" $ARGI
    exit 0
    fi
done

if [ ! $INAMT -ge 1 ] ; then
    echo "Vehicle amount must be >= 1. Exiting now."
    exit 1
fi

if [ ! $OUTAMT -ge 1 ] ; then
    echo "Vehicle amount must be >= 1. Exiting now."
    exit 1
fi

#-------------------------------------------------------------
# Part 2: Build the Shoreside mission file
#-------------------------------------------------------------
SHORE=localhost:9300
nsplug meta_shoreside.moos targ_shoreside.moos -f WARP=$TIME_WARP \
   VNAME="shoreside" 



#-------------------------------------------------------------
# Part 3: Generate random starting positions, speeds and vnames

#-------------------------------------------------------------
if [ "${RANDSTARTIN}" = "true" ] ; then
    pickpos --poly="150,60 : 30,-60 : 50,-60 : 170,60"   --amt=$INAMT   > vinpositions.txt  
    pickpos --amt=$INAMT --spd=1:4 > vinspeeds.txt
    pickpos --amt=$INAMT --vnames=abe,ben,cal,deb,eve  > vinnames.txt
    #pickpos --amt=$INAMT --grps=INBOUND  > vingroups.txt
fi    
if [ "${RANDSTARTOUT}" = "true" ] ; then
    pickpos --poly="180,-180 : 180,-320 : 200,-320 : 200,-180"   --amt=$OUTAMT   > voutpositions.txt  
    pickpos --amt=$OUTAMT --spd=1:4 > voutspeeds.txt
    pickpos --amt=$OUTAMT --vnames=fin,gil,hal,ike,jim  > voutnames.txt
    #pickpos --amt=$OUTAMT --grps=OUTBOUND  > voutgroups.txt
fi
VEHPOSIN=(`cat vinpositions.txt`)
SPEEDSIN=(`cat vinspeeds.txt`)
VNAMESIN=(`cat vinnames.txt`)
#GROOPSIN=(`cat vingroups.txt`)

VEHPOSOUT=(`cat voutpositions.txt`)
SPEEDSOUT=(`cat voutspeeds.txt`)
VNAMESOUT=(`cat voutnames.txt`)
#GROOPSOUT=(`cat voutgroups.txt`)

#-------------------------------------------------------------
# Part 4: Generate the Vehicle mission files
#-------------------------------------------------------------
VNAME1="usv"           # The first vehicle Community
START_POS1="10,-180" 
SHORE_LISTEN="9300"
INPUT_SPEED1="2.5"


nsplug meta_vehicle.moos targ_$VNAME1.moos -f WARP=$TIME_WARP \
    VNAME=$VNAME1          SHARE_LISTEN="9301"              \
    VPORT="9001"           SHORE=$SHORE       \
    START_POS=$START_POS1  


nsplug meta_vehicle.bhv targ_$VNAME1.bhv -f VNAME=$VNAME1     \
    START_POS=$START_POS1 VNAME1=$VNAME1 GROUP=USV  \
    SPEED=$INPUT_SPEED1
#-------------------------------------------------------------
# Part 4a: Generate the Inbound Vehicle mission files
#-------------------------------------------------------------

for INDEXIN in `seq 1 $INAMT`;
do
    ARRAY_INDEXIN=`expr $INDEXIN - 1`
    START_POSIN=${VEHPOSIN[$ARRAY_INDEXIN]}
    VNAMEIN=${VNAMESIN[$ARRAY_INDEXIN]}
    SPEEDIN=${SPEEDSIN[$ARRAY_INDEXIN]}
    #GROUPIN=${GROOPSIN[$ARRAY_INDEXIN]}
    SPEED="${SPEEDIN#speed=*}"
    
    VPORTIN=`expr $INDEXIN + 9001`
    LPORTIN=`expr $INDEXIN + 9301`
     
    echo "Vehicle:" $VNAMEIN "POS:" $START_POSIN "V:" $SPEEDIN         \
      "DB_PORT:" $VPORTIN "PS_PORT:" $LPORTIN

    nsplug meta_vehicle.moos targ_$VNAMEIN.moos -f WARP=$TIME_WARP \
       VNAME=$VNAMEIN   START_POS=$START_POSIN   SHORE=$SHORE    \
       VPORT=$VPORTIN   SHARE_LISTEN=$LPORTIN    GROUP=INBOUND                \
       VTYPE="kayak"  
    
    nsplug meta_vehicle.bhv targ_$VNAMEIN.bhv -f  VNAME=$VNAMEIN     \
       START_POS=$START_POSIN   ORDER=$ORDER   GROUP=INBOUND        \
           ORFER=$ORDER   SPEED=$SPEED      
done

#-------------------------------------------------------------
# Part 4b: Generate the Outbound Vehicle mission files
#-------------------------------------------------------------

for INDEXOUT in `seq 1 $OUTAMT`;
do
    ARRAY_INDEXOUT=`expr $INDEXOUT - 1`
    START_POSOUT=${VEHPOSOUT[$ARRAY_INDEXOUT]}
    VNAMEOUT=${VNAMESOUT[$ARRAY_INDEXOUT]}
    SPEEDOUT=${SPEEDSOUT[$ARRAY_INDEXOUT]}
    #GROUPOUT=${GROOPSOUT[$ARRAY_INDEXOUT]}
    SPEED="${SPEEDOUT#speed=*}"
    
    VPORTOUT=`expr $INDEXOUT + 9007`
    LPORTOUT=`expr $INDEXOUT + 9307`
     
    echo "Vehicle:" $VNAMEOUT "POS:" $START_POSOUT "V:" $SPEEDOUT         \
      "DB_PORT:" $VPORTOUT "PS_PORT:" $LPORTOUT

    nsplug meta_vehicle.moos targ_$VNAMEOUT.moos -f WARP=$TIME_WARP \
       VNAME=$VNAMEOUT   START_POS=$START_POSOUT   SHORE=$SHORE    \
       VPORT=$VPORTOUT   SHARE_LISTEN=$LPORTOUT    GROUP=OUTBOUND                \
       VTYPE="kayak"  
    
    nsplug meta_vehicle.bhv targ_$VNAMEOUT.bhv -f  VNAME=$VNAMEOUT     \
       START_POS=$START_POSOUT   ORDER=$ORDER   GROUP=OUTBOUND        \
           ORFER=$ORDER   SPEED=$SPEED      
done

#-------------------------------------------------------------
# Part 5: Allow to exit now if just want to examine the mission
#         files without launching.
#-------------------------------------------------------------
if [ ${JUST_MAKE} = "yes" ] ; then
    exit 0
fi

#-------------------------------------------------------------
# Part 6: Launch the Shoreside community
#-------------------------------------------------------------
printf "Launching Shoreside MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler targ_shoreside.moos >& /dev/null &
sleep 0.1

#-------------------------------------------------------------
# Part 7: Launch the Vehicle communities
#-------------------------------------------------------------
printf "Launching $VNAME1 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler targ_$VNAME1.moos >& /dev/null &
pSegListIntercept targ_$VNAME1.moos >& /dev/null &
for INDEXIN in `seq 1 $INAMT`;
do 
    ARRAY_INDEXIN=`expr $INDEXIN - 1`
    VNAMEIN=${VNAMESIN[$ARRAY_INDEXIN]}
    printf "Launching $VNAMEIN MOOS Community (WARP=%s) \n" $TIME_WARP
    pAntler targ_$VNAMEIN.moos >& /dev/null &
    sleep 0.1
done

for INDEXOUT in `seq 1 $OUTAMT`;
do 
    ARRAY_INDEXOUT=`expr $INDEXOUT - 1`
    VNAMEOUT=${VNAMESOUT[$ARRAY_INDEXOUT]}
    printf "Launching $VNAMEOUT MOOS Community (WARP=%s) \n" $TIME_WARP
    pAntler targ_$VNAMEOUT.moos >& /dev/null &
    sleep 0.1
done

#-------------------------------------------------------------
# Part 8: Launch uMac until the mission is quit
#-------------------------------------------------------------

uMAC targ_shoreside.moos

printf "Killing all processes ... \n"
mykill
printf "Done killing processes.   \n"

