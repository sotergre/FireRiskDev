#!/bin/bash
#/Volumes/MacOSXJ/DATA/AMZEX/3B42RT/2012/3_hourly/TRMM_AMZEX_GrADS.sh
printf "\nMoving into $DATA_PATH/GLOBAL/IMERG/$year/half_hourly\n"
cd "$DATA_PATH/GLOBAL/IMERG/$year/half_hourly"


export set mo=`date -d "$year-01-01 +$(( $jday - 1 ))days" +%m`

export set DOM=`date -d "$year-01-01 +$(( $jday - 1 ))days" +%d`




for (( min = 0; min <= 1410; min = min + 30 ))
do    
    export set hr=$(date -d "${year}-${mo}-${DOM} +$(( $min*60 ))seconds" +%H)
    
    #IMERG30m_early.AMZ.${year}${mo}${DOM}.${hr}.bin
    
    #gdalwarp -overwrite -s_srs EPSG:4326 -t_srs "+proj=sinu +lon_0=-68.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -r near -ts 622 911 -q -cutline ${DATA_PATH}/AMZEX/MASK/cm_mod07_amzex_mask.bin -dstalpha -of EHdr "${DATA_PATH}/GLOBAL/IMERG/${year}/half_hourly/3B-HHR-E.MS.MRG.3IMERG.${year}${currmonth}${currday}-S$(date -d "${year}-${mo}-${DOM} +$(( $min*60 ))seconds" +%H%M%S)-E$(date -d "${year}-${mo}-${DOM} +$(( $min*60 - 1 ))seconds" +%H%M%S).$(printf "%.4d" $min).V03E.30min.tif" "${DATA_PATH}/AMZEX/IMERG/${year}/half_hourly/3IMERG.${year}${mo}${DOM}.$(printf "%.4d" $min).7.precipitation"
    #gdalwarp -overwrite -s_srs EPSG:4326 -t_srs "+proj=sinu +lon_0=-68.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -r near -ts 622 911 -of EHdr ${DATA_PATH}/AMZEX/3B42RT/${year}/RAW/3B42.${year}${mo}${DOM}.${hr}.7.precipitation.bin ${DATA_PATH}/AMZEX/3B42RT/${year}/3_hourly/3B42.${year}${mo}${DOM}.${hr}.7.precipitation
    printf "\nIMERG30m_early.AMZ.${year}${mo}${DOM}.$(printf "%.4d" $min).bin\n"
    echo;echo;
    echo gdalwarp -overwrite -s_srs EPSG:4326 -t_srs \"+proj=sinu +lon_0=-68.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84\" -r near -ts 622 911 -cutline \"${DATA_PATH}/AMZEX/MASK/cm_mod07_amzex_mask.bin\" -crop_to_cutline -ot Int16 -of EHdr \"${DATA_PATH}/GLOBAL/IMERG/${year}/half_hourly/3B-HHR-E.MS.MRG.3IMERG.${year}${mo}${DOM}-S$(date -d "${year}-${mo}-${DOM} +$(( $min*60 ))seconds" +%H%M%S)-E$(date -d "${year}-${mo}-${DOM} +$(( $min*60 - 1 ))seconds" +%H%M%S).$(printf "%.4d" $min).V03E.30min.tif\" \"${DATA_PATH}/AMZEX/IMERG/${year}/half_hourly/IMERG30m_early.AMZ.${year}${mo}${DOM}.$(printf "%.4d" $min).bin\"
    echo;echo;
    gdalwarp -overwrite -s_srs EPSG:4326 -t_srs "+proj=sinu +lon_0=-68.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -r near -ts 622 911 -cutline "${DATA_PATH}/AMZEX/MASK/cm_mod07_amzex_mask.bin" -crop_to_cutline -ot Int16 -of EHdr "${DATA_PATH}/GLOBAL/IMERG/${year}/half_hourly/3B-HHR-E.MS.MRG.3IMERG.${year}${mo}${DOM}-S$(date -d "${year}-${mo}-${DOM} +$(( $min*60 ))seconds" +%H%M%S)-E$(date -d "${year}-${mo}-${DOM} +$(( $min*60 - 1 ))seconds" +%H%M%S).$(printf "%.4d" $min).V03E.30min.tif" "${DATA_PATH}/AMZEX/IMERG/${year}/half_hourly/IMERG30m_early.AMZ.${year}${mo}${DOM}.$(printf "%.4d" $min).bin"
done

cd "$DATA_PATH/AMZEX/IMERG/$year/half_hourly"
#Arguments: year startjday endjday timeperiod nrow ncols outputdirectory
dursum_IMERG_AMZ_GMT $year $prevjday $prevjday day 911 622 "$DATA_PATH/AMZEX/IMERG/$year/daysum/"
dursum_IMERG_AMZ_GMT $year $prevjday $prevjday p1 911 622 "$DATA_PATH/AMZEX/IMERG/$year/15_hrdur/"
dursum_IMERG_AMZ_GMT $year $prevjday $prevjday p2 911 622 "$DATA_PATH/AMZEX/IMERG/$year/09_hrdur/"



#grads -bpxc "run $DATA_PATH/AMZEX/3B42RT/2012/3_hourly/GrADS_Sum_AMZEX.gs $month $day $year"

exit
