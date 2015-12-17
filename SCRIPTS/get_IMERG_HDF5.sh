#!/bin/bash

export set prevmonth=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%m)
export set prevday=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%d)
prevHMS () { date -d "${prevyear}-${prevmonth}-${prevday} +$(( $min*60 ))seconds" +%H%M%S; }
prevHMS_1 () { date -d "${prevyear}-${prevmonth}-${prevday} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }

export set currmonth=$( date -d "$year-01-01 +$(( $jday - 1 ))days" +%m)
export set currday=$( date -d "$year-01-01 +$(( $jday - 1 ))days" +%d)
currHMS () { date -d "${year}-${currmonth}-${currday} +$(( $min*60 ))seconds" +%H%M%S; }
currHMS_1 () { date -d "${year}-${currmonth}-${currday} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }

echo "begin WGET"

for (( min = 0; min <= 1410; min = min + 30 ))
do
	if [ -e "${DATA_PATH}/GLOBAL/IMERG/${year}/LATE/half_hourly/3B-HHR-L.MS.MRG.3IMERG.${year}${currmonth}${currday}-S$(currHMS)-E$(currHMS_1).$(printf "%.4d" $min).V03E.RT-H5" ]
	then 
		echo "File Up to Date"
	else
		wget -nv -nc --ftp-user=sotergre@msu.edu --ftp-password=sotergre@msu.edu -P${DATA_PATH}/GLOBAL/IMERG/${year}/LATE/half_hourly/ "ftp://198.118.195.61:21/data/imerg/late/${year}${currmonth}/3B-HHR-L.MS.MRG.3IMERG.${year}${currmonth}${currday}-S$(currHMS)-E$(currHMS_1).$(printf "%.4d" $min).V03E.RT-H5"
	fi
done
exit 0

