#!/bin/bash

#####ncftp must be installed prior to executing the following script#####
# get_MOD07.sh reads filenames from textfile "/FIRERISK/DATA/HEMI/MOD07L2/YYYY/MOD07_L2.AYYYYJJJ.txt" 
# and pulls files from ftp site ladsweb.nascom.nasa.gov
# input args are 1-year (YYYY) 2-julian day (JJJ) 
#
# WARNING: HDFLook will fail if there are 3000 or more files in the input directory
#
# $rem is a global variable set in firecast_init_manual
# it represents the 33rd julian day in the past and all MODIS files older than it should be removed
#
# MOD07_L2.*hemi.txt is a text file with a list of MODIS files to download for that day
#   
year=$1
rem=$2
# let "rem = $rem"
echo "HEMI:Removing MOD07_L2.A${year}${rem}"
cd /firecast/DATA/HEMI/MOD07L2/${year}/

# while read file 
# do
#     ncftpget ftp://ladsweb.nascom.nasa.gov/allData/6/MOD07_L2/${year}/${jday}/${file} << FTP
#     FTP
# done < MOD07_L2.A${year}${jday}hemi.txt

while [ -f "MOD07_L2.A${year}${rem}hemi.txt" ]
do
while read file 
do
echo "Removing: $file"
rm ${file} << FTP
FTP
done < MOD07_L2.A${year}${rem}hemi.txt

rm MOD07_L2.A${year}${rem}hemi.txt
let "rem -= 1"
done

exit 0
EOF