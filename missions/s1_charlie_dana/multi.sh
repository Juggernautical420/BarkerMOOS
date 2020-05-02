#!/bin/bash 
for i in {1..60}
do
sh ./launch.sh --in=5 --out=5 5 

sleep 5
uPokeDB targ_shoreside.moos MOOS_MANUAL_OVERRIDE_ALL=false RETURN_ALL=false STATION_KEEP_ALL=false DEPLOY_ALL=true
sleep 5

WAIT_ON_INTERCEPT=$true


if uQueryDB targ_usv.moos --condition="TSS_POPULATE=true"; then
	WAIT_ON_INTERCEPT=$false
fi	

sleep 5

if [ !WAIT_ON_INTERCEPT ]; then
	if uQueryDB targ_usv.moos --condition="FINISHED_RUN=true"; then
		uPokeDB targ_shoreside.moos SCORE=true
	fi
fi


printf "Killing all processes ... \n"
ktm 
sleep 2
ktm
sleep 2
sh ./clean.sh
printf "Done killing processes.   \n"

done