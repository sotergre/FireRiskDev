#!/bin/bash


export set prevmonth=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%m)
export set prevday=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%d)



currmonth=$( date -d "$year-01-01 +$(( $jday - 1 ))days" +%m)
currday=$( date -d "$year-01-01 +$(( $jday - 1 ))days" +%d)


#ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${year}${currmonth}${currday}-S0${hour}0000-E0${hour}2959.$(printf "%.4d" $(( $hour*60 ))).V03E.3hr.tif
#ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${year}${currmonth}${currday}-S0${hour}0000-E0${hour}2959.$(printf "%.4d" $(( $hour*60 ))).V03E.3hr.tfw
echo "IMERG download log for $prevday-$prevmonth-$prevyear (dd-mm-yyyy)"

echo "begin WGET"

for hour in 6 9
do
	wget --ftp-user=sotergre@msu.edu --ftp-password=sotergre@msu.edu -P/firecast/DATA/GLOBAL/IMERG/${prevyear}/3_hourly "ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${prevyear}${prevmonth}${prevday}-S0${hour}0000-E0${hour}2959.$(printf "%.4d" $(( $hour*60 ))).V03E.3hr.tif"
	wget --ftp-user=sotergre@msu.edu --ftp-password=sotergre@msu.edu -P/firecast/DATA/GLOBAL/IMERG/${prevyear}/3_hourly "ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${prevyear}${prevmonth}${prevday}-S0${hour}0000-E0${hour}2959.$(printf "%.4d" $(( $hour*60 ))).V03E.3hr.tfw"
done

for hour in 12 15 18 21
do
	wget --ftp-user=sotergre@msu.edu --ftp-password=sotergre@msu.edu -P/firecast/DATA/GLOBAL/IMERG/${prevyear}/3_hourly "ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${prevyear}${prevmonth}${prevday}-S${hour}0000-E${hour}2959.$(printf "%.4d" $(( $hour*60 ))).V03E.3hr.tif"
	wget --ftp-user=sotergre@msu.edu --ftp-password=sotergre@msu.edu -P/firecast/DATA/GLOBAL/IMERG/${prevyear}/3_hourly "ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${prevyear}${prevmonth}${prevday}-S${hour}0000-E${hour}2959.$(printf "%.4d" $(( $hour*60 ))).V03E.3hr.tfw"
done

for hour in 0 3
do
	wget --ftp-user=sotergre@msu.edu --ftp-password=sotergre@msu.edu -P/firecast/DATA/GLOBAL/IMERG/${year}/3_hourly "ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${year}${currmonth}${currday}-S0${hour}0000-E0${hour}2959.$(printf "%.4d" $(( $hour*60 ))).V03E.3hr.tif"
	wget --ftp-user=sotergre@msu.edu --ftp-password=sotergre@msu.edu -P/firecast/DATA/GLOBAL/IMERG/${year}/3_hourly "ftp://198.118.195.61:21/data/imerg/gis/early/3B-HHR-E.MS.MRG.3IMERG.${year}${currmonth}${currday}-S0${hour}0000-E0${hour}2959.$(printf "%.4d" $(( $hour*60 ))).V03E.3hr.tfw"
done

exit

