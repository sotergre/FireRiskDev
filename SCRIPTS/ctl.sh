#!/bin/bash

printf "\nCTL file written for ${year}${mo}${DOM}:${hr}\n"

cd "$DATA_PATH/GLOBAL/3B42RT/$year/3_hourly"
f="3B42RT.${year}${mo}${DOM}${hr}.7R2.bin"


echo "dset ^$f" > "$f.ctl"
echo "options big_endian yrev" >> "$f.ctl"
echo "title Real-Time Three Hourly TRMM and Other Satellite Rainfall 3B42RT" >> "$f.ctl"
echo "headerbytes 2880" >> "$f.ctl"
echo "undef -31999" >> "$f.ctl"
echo "xdef 1440 linear 0.125 0.25" >> "$f.ctl"
echo "ydef 480  linear -59.875 0.25" >> "$f.ctl"
echo "zdef 1 levels 1" >> "$f.ctl"
echo "tdef 1 linear ${hr}Z${DOM}jan${year} 3hr" >> "$f.ctl"
echo "vars 3" >> "$f.ctl"
echo "p       0 -1,40,2,-1 Precipitation mm/hr" >> "$f.ctl"
echo "perror  0 -1,40,2,-1 Precipitation Error mm/hr" >> "$f.ctl"
echo "source  0 -1,40,1    Source Number" >> "$f.ctl"
echo "endvars" >> "$f.ctl"

exit
