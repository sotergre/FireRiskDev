#!/bin/bash
#/Volumes/MacOSXJ/DATA/AMZEX/3B42RT/2012/3_hourly/TRMM_AMZEX_GrADS.sh
printf "\nMoving into $DATA_PATH/GLOBAL/3B42RT/$year/3_hourly\n"
cd "$DATA_PATH/GLOBAL/3B42RT/$year/3_hourly"


export set mo=`date -d "$year-01-01 +$(( $jday - 1 ))days" +%m`

export set DOM=`date -d "$year-01-01 +$(( $jday - 1 ))days" +%d`




for hour in "00" "03" "06" "09" "12" "15" "18" "21"
do    
    export set hr=$hour
    ctl_Flag=`$SCRIPTS_PATH/ctl.sh`
    grads -bcpx "run $DATA_PATH/GLOBAL/3B42RT/$year/3_hourly/TRMMsingle.gs ${year}${mo}${DOM}${hr}"
    mv "$DATA_PATH/GLOBAL/3B42RT/$year/3_hourly/3B42RT.${year}${mo}${DOM}${hr}.7R2.dat" "$DATA_PATH/AMZEX/3B42RT/$year/RAW/3B42.${year}${mo}${DOM}.${hr}.7.precipitation.bin"
    mv "$DATA_PATH/GLOBAL/3B42RT/$year/3_hourly/3B42RT.${year}${mo}${DOM}${hr}.7R2.ctl" "$DATA_PATH/AMZEX/3B42RT/$year/RAW/3B42.${year}${mo}${DOM}.${hr}.7.precipitation.ctl"
    cp "$DATA_PATH/gtifheader/TRMM.hdr" "$DATA_PATH/AMZEX/3B42RT/$year/RAW/3B42.${year}${mo}${DOM}.${hr}.7.precipitation.hdr"
    gdalwarp -s_srs EPSG:4326 -t_srs "+proj=sinu +lon_0=-68.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -ts 622 911 -r near -of EHdr -overwrite ${DATA_PATH}/AMZEX/3B42RT/${year}/RAW/3B42.${year}${mo}${DOM}.${hr}.7.precipitation.bin ${DATA_PATH}/AMZEX/3B42RT/${year}/3_hourly/3B42.${year}${mo}${DOM}.${hr}.7.precipitation
done

cd "$DATA_PATH/AMZEX/3B42RT/$year/3_hourly"
#Arguments: year startjday endjday timeperiod nrow ncols outputdirectory
dursum_3B42RT_AMZEX_32_win2 $year $jday $jday day 911 622 "$DATA_PATH/AMZEX/3B42RT/$year/daysum/"
dursum_3B42RT_AMZEX_32_win2 $year $jday $jday p1 911 622 "$DATA_PATH/AMZEX/3B42RT/$year/15_hrdur/"
dursum_3B42RT_AMZEX_32_win2 $year $jday $jday p2 911 622 "$DATA_PATH/AMZEX/3B42RT/$year/09_hrdur/"



#grads -bpxc "run $DATA_PATH/AMZEX/3B42RT/2012/3_hourly/GrADS_Sum_AMZEX.gs $month $day $year"

exit
