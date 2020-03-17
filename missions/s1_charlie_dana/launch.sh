#!/bin/bash 
#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
TIME_WARP=1
JUST_MAKE="no"
RANDSTART="true"
ORDER="normal"
AMT=1
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
    printf "%s [SWITCHES] [time_warp]          \n" $0
    printf "  --just_make, -j                  \n" 
    printf "  --help, -h                       \n" 
    printf "  --amt=N                          \n" 
    printf "  --norand                         \n" 
    exit 0;
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
    JUST_MAKE="yes"
    elif [ "${ARGI}" = "--rand" -o "${ARGI}" = "-r" ] ; then
    RANDSTART="true"
    elif [ "${ARGI:0:6}" = "--amt=" ] ; then
        AMT="${ARGI#--amt=*}"
    else 
    printf "Bad Argument: %s \n" $ARGI
    exit 0
    fi
done

if [ ! $AMT -ge 1 ] ; then
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
#         NEAST Poly: 90,20 : 150,30 : 200,-25 : 160,-65, 90,-15
#         WEST  Poly: -30,-30 : -30,-135 : 15,-135 : 10,-30
#         SEAST Poly: 145,-120 : 170,-135 : 140,-175 : 125,-160
#-------------------------------------------------------------
if [ "${RANDSTART}" = "true" ] ; then
    pickpos --poly="-10,-200:30,-200:20,-160:-10,-160"      \
            --poly="150,60 : 30,-60 : 50,-60 : 170,60"  \
            --poly="180,-180 : 180,-350 : 200,-350 : 200,-180"  \
        --amt=$AMT   > vpositions.txt  
    pickpos --amt=$AMT --spd=1:4 > vspeeds.txt
    pickpos --amt=$AMT --vnames  > vnames.txt
    pickpos --amt=$AMT --grps=UUV,INBOUND,OUTBOUND:alt  > vgroups.txt
fi
VEHPOS=(`cat vpositions.txt`)
SPEEDS=(`cat vspeeds.txt`)
VNAMES=(`cat vnames.txt`)
GROOPS=(`cat vgroups.txt`)


#-------------------------------------------------------------
# Part 4: Generate the Vehicle mission files
#-------------------------------------------------------------
for INDEX in `seq 1 $AMT`;
do
    ARRAY_INDEX=`expr $INDEX - 1`
    START_POS=${VEHPOS[$ARRAY_INDEX]}
    VNAME=${VNAMES[$ARRAY_INDEX]}
    SPEED=${SPEEDS[$ARRAY_INDEX]}
    GROUP=${GROOPS[$ARRAY_INDEX]}
    SPEED="${SPEED#speed=*}"
    
    VPORT=`expr $INDEX + 9000`
    LPORT=`expr $INDEX + 9300`
     
    echo "Vehicle:" $VNAME "POS:" $START_POS "V:" $SPEED         \
      "DB_PORT:" $VPORT "PS_PORT:" $LPORT

    nsplug meta_vehicle.moos targ_$VNAME.moos -f WARP=$TIME_WARP \
       VNAME=$VNAME   START_POS=$START_POS   SHORE=$SHORE    \
       VPORT=$VPORT   SHARE_LISTEN=$LPORT    GROUP=$GROUP                \
       VTYPE="kayak"  
    
    nsplug meta_vehicle.bhv targ_$VNAME.bhv -f  VNAME=$VNAME     \
       START_POS=$START_POS   ORDER=$ORDER   GROUP=$GROUP        \
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
for INDEX in `seq 1 $AMT`;
do 
    ARRAY_INDEX=`expr $INDEX - 1`
    VNAME=${VNAMES[$ARRAY_INDEX]}
    printf "Launching $VNAME MOOS Community (WARP=%s) \n" $TIME_WARP
    pAntler targ_$VNAME.moos >& /dev/null &
    sleep 0.1
done

#-------------------------------------------------------------
# Part 8: Launch uMac until the mission is quit
#-------------------------------------------------------------

uMAC targ_shoreside.moos

printf "Killing all processes ... \n"
mykill
printf "Done killing processes.   \n"


