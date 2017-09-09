#!/bin/bash

#IMERG3 data tables and output grid remained unchanged for V04B. So all that we needed to change was the filename.
#This may not be the case for next versions. So read the PPS changelog when notified of version update.
export set ver="V04B"
export set arc=`arch`
if [ "$arc" = "i386" ]
then
    ### BSD date command ###
    export set prevmonth=$( gdate -d "$year-01-01 +$(( $jday - 2 ))days" +%m)
    export set prevday=$( gdate -d "$year-01-01 +$(( $jday - 2 ))days" +%d)
    prevHMS () { gdate -d "${prevyear}-${prevmonth}-${prevday} +$(( $min*60 ))seconds" +%H%M%S; }
    prevHMS_1 () { gdate -d "${prevyear}-${prevmonth}-${prevday} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }
 
    export set currmonth=$( gdate -d "$year-01-01 +$(( $jday - 1 ))days" +%m)
    export set currday=$( gdate -d "$year-01-01 +$(( $jday - 1 ))days" +%d)
    currHMS () { gdate -d "${year}-${currmonth}-${currday} +$(( $min*60 ))seconds" +%H%M%S; }
    currHMS_1 () { gdate -d "${year}-${currmonth}-${currday} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }
else
    ### Linux date command ###
    export set prevmonth=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%m)
    export set prevday=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%d)
    prevHMS () { date -d "${prevyear}-${prevmonth}-${prevday} +$(( $min*60 ))seconds" +%H%M%S; }
    prevHMS_1 () { date -d "${prevyear}-${prevmonth}-${prevday} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }
 
    export set currmonth=$( date -d "$year-01-01 +$(( $jday - 1 ))days" +%m)
    export set currday=$( date -d "$year-01-01 +$(( $jday - 1 ))days" +%d)
    currHMS () { date -d "${year}-${currmonth}-${currday} +$(( $min*60 ))seconds" +%H%M%S; }
    currHMS_1 () { date -d "${year}-${currmonth}-${currday} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }
fi
echo "prevmonth=$prevmonth"
echo "prevday=$prevday"
echo "currmonth=$currmonth"
echo "currday=$currday"
#read -n1 -r -p "..."
echo "begin WGET"

for (( min = 0; min <= 1410; min = min + 30 ))
do
	if [ -e "${DATA_PATH}/GLOBAL/IMERG/${year}/LATE/half_hourly/3B-HHR-L.MS.MRG.3IMERG.${year}${currmonth}${currday}-S$(currHMS)-E$(currHMS_1).$(printf "%.4d" $min).$ver.RT-H5" ]
	then 
		echo "File Up to Date"
	else
		wget -nv -nc --ftp-user=sotergre@msu.edu --ftp-password=sotergre@msu.edu -P${DATA_PATH}/GLOBAL/IMERG/${year}/LATE/half_hourly/ "ftp://198.118.195.61:21/data/imerg/late/${year}${currmonth}/3B-HHR-L.MS.MRG.3IMERG.${year}${currmonth}${currday}-S$(currHMS)-E$(currHMS_1).$(printf "%.4d" $min).$ver.RT-H5"
	fi
done
exit 0

