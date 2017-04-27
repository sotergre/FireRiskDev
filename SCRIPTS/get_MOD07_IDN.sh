#!/bin/bash
 
#####ncftp must be installed prior to executing the following script#####
#get_MOD07.sh reads filenames from textfile "/FIRERISK/DATA/IDN/MOD07L2/YYYY/MOD07_L2.AYYYYJJJ.txt" 
#and pulls files from ftp site ladsweb.nascom.nasa.gov
#input args are 1-year (YYYY) 2-julian day (JJJ)

year=$1
jday=$2
echo "IDN:Getting MOD07_L2.A${year}${jday}"

cd /firecast/DATA/IDN/MOD07L2/${year}/
#$echo "DEBUG:MOD07_L2.A${year}$(printf "%.3d" $jday)idn.txt"
#cat "MOD07_L2.A${year}$(printf "%.3d" $jday)idn.txt"

while read file 
do
ncftpget ftp://ladsweb.nascom.nasa.gov/allData/6/MOD07_L2/${year}/${jday}/${file} << FTP
FTP
done < MOD07_L2.A${year}${jday}idn.txt

#done < /firecast/MOD07_L2.A2015246idn.txt
exit 0