#!/bin/zsh

prevyear=2009
prevmonth="02"
prevday="16"
prevjday=47

curryear=2009
currmonth="02"
currday="17"



echo "TRMM 3B42RT download log for $prevday-$prevmonth-$prevyear (dd-mm-yyyy)"

echo "begin WGET"

for hour in 6 9
do
	wget -P/FIRERISK/DATA/GLOBAL/3B42RT/${prevyear}/3_hourly ftp://trmmopen.gsfc.nasa.gov/pub/merged/mergeIRMicro/3B42RT.${prevyear}${prevmonth}${prevday}0${hour}.6.bin.gz
done

for hour in 12 15 18 21
do
	wget -P/FIRERISK/DATA/GLOBAL/3B42RT/${prevyear}/3_hourly ftp://trmmopen.gsfc.nasa.gov/pub/merged/mergeIRMicro/3B42RT.${prevyear}${prevmonth}${prevday}${hour}.6.bin.gz
done

for hour in 0 3
do
	wget -P/FIRERISK/DATA/GLOBAL/3B42RT/${curryear}/3_hourly ftp://trmmopen.gsfc.nasa.gov/pub/merged/mergeIRMicro/3B42RT.${curryear}${currmonth}${currday}0${hour}.6.bin.gz
done


#unzip files and remove the zipped files
gunzip -f /FIRERISK/DATA/GLOBAL/3B42RT/${prevyear}/3_hourly/3B42RT.${prevyear}${prevmonth}${prevday}*.gz
gunzip -f /FIRERISK/DATA/GLOBAL/3B42RT/${curryear}/3_hourly/3B42RT.${curryear}${currmonth}${currday}*.gz
