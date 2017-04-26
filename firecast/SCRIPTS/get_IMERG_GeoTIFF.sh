#!/bin/bash


export set prevmonth=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%m)
export set prevday=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%d)
prevHMS () { date -d "${prevyear}-${prevmonth}-${prevday} +$(( $min*60 ))seconds" +%H%M%S; }
prevHMS_1 () { date -d "${prevyear}-${prevmonth}-${prevday} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }



export set currmonth=$( date -d "$year-01-01 +$(( $jday - 1 ))days" +%m)
export set currday=$( date -d "$year-01-01 +$(( $jday - 1 ))days" +%d)
currHMS () { date -d "${year}-${currmonth}-${currday} +$(( $min*60 ))seconds" +%H%M%S; }
currHMS_1 () { date -d "${year}-${currmonth}-${currday} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }

#date -d "$year-01-01 +${min}minutes +%H%M%S
#ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${year}${currmonth}${currday}-S0${hour}0000-E0${hour}2959.$(printf "%.4d" $(( $hour*60 ))).V03E.3hr.tif
#ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${year}${currmonth}${currday}-S0${hour}0000-E0${hour}2959.$(printf "%.4d" $(( $hour*60 ))).V03E.3hr.tfw
echo "IMERG download log for $prevday-$prevmonth-$prevyear (dd-mm-yyyy)"

echo "begin WGET"

for (( min = 0; min <= 1410; min = min + 30 ))
do
	if [ "$min" -le 180 ]
	then
		wget --ftp-user=sotergre@msu.edu --ftp-password=sotergre@msu.edu -P/firecast/DATA/GLOBAL/IMERG/${year}/half_hourly "ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${year}${currmonth}${currday}-S$(currHMS)-E$(currHMS_1).$(printf "%.4d" $min).V03E.30min.tif"
		wget --ftp-user=sotergre@msu.edu --ftp-password=sotergre@msu.edu -P/firecast/DATA/GLOBAL/IMERG/${year}/half_hourly "ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${year}${currmonth}${currday}-S$(currHMS)-E$(currHMS_1).$(printf "%.4d" $min).V03E.30min.tfw"
	else
		wget --ftp-user=sotergre@msu.edu --ftp-password=sotergre@msu.edu -P/firecast/DATA/GLOBAL/IMERG/${prevyear}/half_hourly "ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${prevyear}${prevmonth}${prevday}-S$(prevHMS)-E$(prevHMS_1).$(printf "%.4d" $min).V03E.30min.tif"
		wget --ftp-user=sotergre@msu.edu --ftp-password=sotergre@msu.edu -P/firecast/DATA/GLOBAL/IMERG/${prevyear}/half_hourly "ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${prevyear}${prevmonth}${prevday}-S$(prevHMS)-E$(prevHMS_1).$(printf "%.4d" $min).V03E.30min.tfw"
	fi


done

exit

