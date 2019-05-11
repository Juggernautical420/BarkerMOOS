#!/bin/bash -e
#---------------------------------------------------------
# File: launch_vehicle.sh
# Name: Mike Benjamin
# Date: May 8th, 2019
# Note: Goal of general pavilion vehicle launch script
#---------------------------------------------------------
#  Part 1: Initialize configurable variables with defaults
#---------------------------------------------------------
TIME_WARP=1
JUST_MAKE="no"
HELP="no"
SIM="false"
INDEX=0
STARTPOS="0,0,180"
RETURN_POS="6,-6"
INTERACTIVE="true"

SHORE_IPADDR=192.168.10.100
SHORE_PSHARE=9200

VNAMES=(evan felix gus hal ida jing)
COOL_FAC=50
COOL_STEPS=1000
CONCURRENT="true"
ADAPTIVE="false"
SURVEY_X=70
SURVEY_Y=-100
HEIGHT1=150
WIDTH1=120
LANE_WIDTH1=25
DEGREES1=0



#---------------------------------------------------------
#  Part 2: Check for and handle command-line arguments
#---------------------------------------------------------
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	HELP="yes"
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then
        TIME_WARP=$ARGI
    elif [ "${ARGI}" = "--just_make" -o "${ARGI}" = "-j" ] ; then
        JUST_MAKE="yes"
    elif [ "${ARGI}" = "--sim" -o "${ARGI}" = "-s" ] ; then
        SIM="true"
    elif [ "${ARGI}" = "--noui" ] ; then
        INTERACTIVE="false"
    elif [ "${ARGI:0:6}" = "--cool" ] ; then
        COOL_FAC="${ARGI#--cool=*}"
        UNDEFINED_ARG=""
    elif [ "${ARGI}" = "--unconcurrent" -o "${ARGI}" = "-uc" ] ; then
        CONCURRENT="false"
        UNDEFINED_ARG=""
    elif [ "${ARGI}" = "--adaptive" -o "${ARGI}" = "-a" ] ; then
        ADAPTIVE="true"
        UNDEFINED_ARG=""                        
    elif [ "${ARGI:0:12}" = "--start_pos=" ] ; then
        START_POS="${ARGI#--start_pos=*}"
    elif [ "${ARGI:0:10}" = "--shoreip=" ] ; then
        SHORE_IPADDR="${ARGI#--shoreip=*}"
    elif [ "${ARGI:0:10}" = "--shoreps=" ] ; then
        SHORE_PSHARE="${ARGI#--shoreps=*}"
    # elif [ "${ARGI}" = "-v1"  -o "${ARGI}" = "-a" ]; then INDEX=1
    # elif [ "${ARGI}" = "-v2"  -o "${ARGI}" = "-b" ]; then INDEX=2
    # elif [ "${ARGI}" = "-v3"  -o "${ARGI}" = "-c" ]; then INDEX=3
    # elif [ "${ARGI}" = "-v4"  -o "${ARGI}" = "-d" ]; then INDEX=4
    elif [ "${ARGI}" = "-v5"  -o "${ARGI}" = "-e" ]; then INDEX=1
    elif [ "${ARGI}" = "-v6"  -o "${ARGI}" = "-f" ]; then INDEX=2
    elif [ "${ARGI}" = "-v7"  -o "${ARGI}" = "-g" ]; then INDEX=3
    elif [ "${ARGI}" = "-v8"  -o "${ARGI}" = "-H" ]; then INDEX=4
    elif [ "${ARGI}" = "-v9"  -o "${ARGI}" = "-i" ]; then INDEX=5
    elif [ "${ARGI}" = "-v10" -o "${ARGI}" = "-J" ]; then INDEX=6
    # elif [ "${ARGI}" = "-v11" -o "${ARGI}" = "-k" ]; then INDEX=11
    # elif [ "${ARGI}" = "-v12" -o "${ARGI}" = "-l" ]; then INDEX=12
    else
        echo "Bad arg:" $ARGI "Run with -h for help. Exiting (1)."
        exit 1
    fi
done

#---------------------------------------------------------
#  Part 3: Produce the help message if it was requested
#---------------------------------------------------------
if [ ${HELP} = "yes" ] ; then
    echo "$0 [SWITCHES]"
    echo "  --help, -h         Show this help message            "        
    echo "  --just_make, -j    Just make targ files, dont launch "
    echo "  --sim, -s          Simulation mode                   "
    echo "  --noui             Non-interactive, no uMAC launched "
    echo "  --shoreip=<addr>   Try IP address for shoreside      "
    echo "  --shoreps=<addr>   Try shoreside pshare port (Def 9200) "
    # echo "   -v1, or -a        Abe vehicle           "
    # echo "   -v2, or -b        Ben vehicle           "
    # echo "   -v3, or -c        Cal vehicle           "
    # echo "   -v4, or -d        Deb vehicle           "
    echo "   -v5, or -e        Evan vehicle          "
    echo "   -v6, or -f        Felix vehicle         "
    echo "   -v7, or -g        Gus vehicle           "
    echo "   -v8, or -H        Hal vehicle           "
    echo "   -v9, or -i        Ida vehicle           "
    echo "  -v10, or -J        Jing vehicle          "
    # echo "  -v11, or -k        Kirk vehicle          "
    # echo "  -v12, or -l        Luke vehicle          "
    echo "  --startpos=<pos>   For example 10,10,90  "
    echo "  --adaptive, or -a  Adaptive Annealing    "
    echo "  --unconcurrent, or -uc  Unconcurrent Annealing "  
    echo "  --cool             Cooling Steps         "  
    exit 0;
fi

#---------------------------------------------------------
#  Part 4: Handle Ill-formed command-line arguments
#---------------------------------------------------------
if [ ${SIM} = "false" -a ! "$TIME_WARP" = 1 ] ; then
    echo "Time Warp must be 1 unless in sim mode. Exiting (2)."
    exit 2
fi
    
if [ $INDEX -lt 1 -o $INDEX -gt 12 ] ; then
    echo "No vehicle has been selected. Exiting (3)."
    exit 3
fi

#---------------------------------------------------------
#  Part 5: Build out key shell vars based on vehicle index
#---------------------------------------------------------

M200_IP=192.168.$INDEX.1
VNAME=${VNAMES[$INDEX-1]}

# For BOT_SHARE and BOT_MOOSDB convert index "N" to "0N"
if [ $INDEX -lt 10 ] ; then
    INDEX="0"$INDEX
fi
BOT_PSHARE="92$INDEX"
BOT_MOOSDB="90$INDEX"

#---------------------------------------------------------
#  Part 6: Create the .moos and .bhv files.
#    Note: Failed nsplug will abort launch due to bash -e on line 1
#    Note: Undef macros will be aletered to user with nsplug -i flag
#---------------------------------------------------------
nsplug meta_vehicle.moos targ_${VNAME}.moos -f -i              \
       WARP=$TIME_WARP             VNAME=$VNAME                \
       BOT_PSHARE=$BOT_PSHARE      SHORE_PSHARE=$SHORE_PSHARE  \
       BOT_MOOSDB=$BOT_MOOSDB      SHORE_IPADDR=$SHORE_IPADDR  \
       START_POS=$START_POS        HOSTIP_FORCE="localhost"    \
       M200_IP=$M200_IP            SIM=$SIM                    \
       VTYPE=KAYAK \
       COOL_FAC=$COOL_FAC  COOL_STEPS=$COOL_STEPS\
       CONCURRENT=$CONCURRENT  ADAPTIVE=$ADAPTIVE        

nsplug meta_vehicle.bhv targ_${VNAME}.bhv -f -i \
        START_POS=$START_POS SURVEY_X=$SURVEY_X SURVEY_Y=$SURVEY_Y \
        HEIGHT=$HEIGHT1   WIDTH=$WIDTH1 LANE_WIDTH=$LANE_WIDTH1 \
        DEGREES=$DEGREES1 VNAME1=$VNAME1 VNAME2=$VNAME2\
        RETURN_POS=${RETURN_POS}


#---------------------------------------------------------
#  Part 7: Possibly exit now if we're just building targ files
#---------------------------------------------------------
if [ ${JUST_MAKE} = "yes" ] ; then
    echo "Files assembled; vehicle not launched; exiting per request."
    echo "  Local variable set: "
    echo "                 SIM:"$SIM
    echo "             M200_IP:"$M200_IP
    echo "        Vehicle name:"$VNAME
    echo " Vehicle MOOSDB Port:"$BOT_MOOSDB
    echo " Vehicle pshare port:"$BOT_PSHARE
    echo "     Try Shoresie IP:"$SHORE_IPADDR
    echo "Note: If you were trying to launch Jing, use -J (not -j)"
    exit 0
fi

#---------------------------------------------------------
#  Part 8: Launch the processes
#---------------------------------------------------------

echo "Launching $VNAME MOOS Community "
pAntler targ_${VNAME}.moos >& /dev/null &

if [ "${INTERACTIVE}" = "true" ] ; then
    uMAC targ_${VNAME}.moos
    echo "Killing all processes ..."
    kill -- -$$
    echo "Done killing processes."
fi

