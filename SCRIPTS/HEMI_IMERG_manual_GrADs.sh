#!/bin/bash
export set ver="V04B"

printf "\nHEMI:\nMoving into $DATA_PATH/GLOBAL/IMERG/$year/LATE/half_hourly\n"
cd "$DATA_PATH/GLOBAL/IMERG/$year/LATE/half_hourly"
export set arc=`arch`
if [ "$arc" = "i386" ]
then
    export set mo=`gdate -d "$year-01-01 +$(( $jday - 1 ))days" +%m`
    export set DOM=`gdate -d "$year-01-01 +$(( $jday - 1 ))days" +%d`
    export set prevmo=`gdate -d "$year-01-01 +$(( $jday - 2 ))days" +%m`
    export set prevDOM=`gdate -d "$year-01-01 +$(( $jday - 2 ))days" +%d`

    currHMS () { gdate -d "${year}-${mo}-${DOM} +$(( $min*60 ))seconds" +%H%M%S; }
    currHMS_1 () { gdate -d "${year}-${mo}-${DOM} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }
    prevHMS () { gdate -d "${prevyear}-${prevmo}-${prevDOM} +$(( $min*60 ))seconds" +%H%M%S; }
    prevHMS_1 () { gdate -d "${prevyear}-${prevmo}-${prevDOM} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }
else
    export set mo=`date -d "$year-01-01 +$(( $jday - 1 ))days" +%m`
    export set DOM=`date -d "$year-01-01 +$(( $jday - 1 ))days" +%d`
    export set prevmo=`date -d "$year-01-01 +$(( $jday - 2 ))days" +%m`
    export set prevDOM=`date -d "$year-01-01 +$(( $jday - 2 ))days" +%d`

    currHMS () { date -d "${year}-${mo}-${DOM} +$(( $min*60 ))seconds" +%H%M%S; }
    currHMS_1 () { date -d "${year}-${mo}-${DOM} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }
    prevHMS () { date -d "${prevyear}-${prevmo}-${prevDOM} +$(( $min*60 ))seconds" +%H%M%S; }
    prevHMS_1 () { date -d "${prevyear}-${prevmo}-${prevDOM} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }
fi

cd /
for (( min = 0; min <= 1410; min = min + 30 ))
do    
    export set dataset="${DATA_PATH}/GLOBAL/IMERG/${year}/LATE/half_hourly/3B-HHR-L.MS.MRG.3IMERG.${year}${mo}${DOM}-S$(currHMS)-E$(currHMS_1).$(printf "%.4d" $min).$ver.RT-H5"
    
    echo $dataset
    if [ -e "$dataset" ] 
    then
		grads -bclx "run ${SCRIPTS_PATH}/IMERGsingle_HEMI.gs $dataset"
# 		cp "$DATA_PATH/gtifheader/IMERG.hdr" "${dataset}.hdr"
# 		cp "${dataset}.cal.dat" "${dataset}.bin"
#       echo "gdal_translate -a_nodata \"-9999.9\" -projwin -180.0 13.0 180.0 -27.0 -a_ullr -180 13 180 -27 -of EHdr -ot Float32 \"${dataset}.bin\" \"$DATA_PATH/HEMI/IMERG/$year/RAW/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).bin\""
#       gdal_translate -a_nodata "-9999.9" -projwin -180.0 13.0 180.0 -27.0 -a_ullr -180 13 180 -27 -of EHdr -ot Float32 "${dataset}.bin" "$DATA_PATH/HEMI/IMERG/$year/RAW/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).bin"
		cp "${dataset}.cal.dat" "$DATA_PATH/HEMI/IMERG/$year/RAW/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).bin"
		cp "${dataset}.cal.ctl" "$DATA_PATH/HEMI/IMERG/$year/RAW/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).ctl"
		cp "$DATA_PATH/HEMI/gtifheader/IMERG_HEMI.hdr" "$DATA_PATH/HEMI/IMERG/$year/RAW/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).hdr"
		echo "gdalwarp -t_srs EPSG:4326 --debug \"on\" -ts 8004 915 -r near -of EHdr -ot Float32 -overwrite  ${DATA_PATH}/HEMI/IMERG/${year}/RAW/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).bin ${DATA_PATH}/HEMI/IMERG/${year}/half_hourly/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).bin"
		gdalwarp -t_srs EPSG:4326 --debug "on" -ts 8004 915 -r near -of EHdr -ot Float32 -overwrite  ${DATA_PATH}/HEMI/IMERG/${year}/RAW/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).bin ${DATA_PATH}/HEMI/IMERG/${year}/half_hourly/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).bin
# 		gdalwarp -t_srs EPSG:4326 --debug "on" -ts 8004 915 -r near -of GTiff -ot Float32 -overwrite  ${DATA_PATH}/HEMI/IMERG/${year}/RAW/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).bin ${DATA_PATH}/HEMI/IMERG/${year}/half_hourly/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).tif
# 		gdalwarp -t_srs EPSG:4326 --debug "on" -ts 8004 915 -r near -of ENVI -ot Float32 -overwrite  ${DATA_PATH}/HEMI/IMERG/${year}/RAW/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).bin ${DATA_PATH}/HEMI/IMERG/${year}/half_hourly/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).env

#         cp "$DATA_PATH/HEMI/gtifheader/MaskProj.prj" "${DATA_PATH}/HEMI/IMERG/${year}/half_hourly/IMERG30m_early.HEMI.${year}${mo}${DOM}.$(printf "%.4d" $min).prj"
    fi
done

cd "$DATA_PATH/HEMI/IMERG/$year/half_hourly"
#Arguments: year startjday endjday timeperiod nrow ncols outputdirectory
dursum_IMERGHDF_HEMI $year $jday $jday day 915 8004 "$DATA_PATH/HEMI/IMERG/$year/daysum/"
dursum_IMERGHDF_HEMI $year $jday $jday p1 915 8004 "$DATA_PATH/HEMI/IMERG/$year/15_hrdur/"
dursum_IMERGHDF_HEMI $year $jday $jday p2 915 8004 "$DATA_PATH/HEMI/IMERG/$year/09_hrdur/"

cd "$HOME"
exit 0
