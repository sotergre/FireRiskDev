#!/bin/bash

#date -v1d -v1m -v${year}y -v+$(expr $jday - 2)d "+%m"
export set prevmonth=$( date -v1d -v1m -v${prevyear}y -v+$(expr $jday - 2)d "+%m")
export set prevday=$( date -v1d -v1m -v${prevyear}y -v+$(expr $jday - 2)d "+%d")
prevHMS () { date -v${prevyear}y -v${prevmonth}m -v${prevday}d +$(( $min*60 ))s "+%H%M%S"; }
prevHMS_1 () { date -v${prevyear}y -v${prevmonth}m -v${prevday}d +$(( (min+30)*60 - 1 ))s" +%H%M%S"; }



export set currmonth=$( date -v1d -v1m -v${year}y -v+$(expr $jday - 1)d "+%m" +%m)
export set currday=$( date -v1d -v1m -v${year}y -v+$(expr $jday - 1)d "+%d")
currHMS () { date -v${year}y -v${currmonth}m -v${currday}d +$(( $min*60 ))s "+%H%M%S"; }
currHMS_1 () { date -v${year}y -v${currmonth}m -v${currday}d +$(( (min+30)*60 - 1 )) "+%H%M%S"; }

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

