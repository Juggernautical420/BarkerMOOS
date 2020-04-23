#!/bin/bash 
for i in {1..50}
do
sh ./launch.sh --in=5 --out=5 15 

sleep 5
uPokeDB targ_shoreside.moos MOOS_MANUAL_OVERRIDE_ALL=false RETURN_ALL=false STATION_KEEP_ALL=false DEPLOY_ALL=true
sleep 5



if uQueryDB targ_usv.moos --condition="WPT_INDEX=2" --wait=3600; then
	uPokeDB targ_shoreside.moos SCORE=true
fi


printf "Killing all processes ... \n"
ktm 
sleep 2
ktm
sleep 2
sh ./clean.sh
printf "Done killing processes.   \n"

done