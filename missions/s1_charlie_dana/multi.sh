#!/bin/bash 
for i in {1..5}
do
sh ./launch.sh --in=1 --out=4 15 

sleep 5
uPokeDB targ_shoreside.moos MOOS_MANUAL_OVERRIDE_ALL=false RETURN_ALL=false STATION_KEEP_ALL=false DEPLOY_ALL=true
sleep 10

WAIT_ON_INTERCEPT=$true
WAIT_ON_WPT=$true

if uQueryDB targ_usv.moos --condition="TSS_POPULATE=true"; then
	WAIT_ON_INTERCEPT=$false
fi	

sleep 10

if [ !WAIT_ON_INTERCEPT ]; then
	if uQueryDB targ_usv.moos --condition="WPT_INDEX=2" --wait=8400; then
		WAIT_ON_WPT=$false
	fi
fi

sleep 10

if [ !WAIT_ON_WPT ]; then 
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