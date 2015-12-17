#!/bin/bash

printf "\nMoving into $DATA_PATH/GLOBAL/IMERG/$year/LATE/half_hourly\n"
cd "$DATA_PATH/GLOBAL/IMERG/$year/LATE/half_hourly"


export set mo=`date -d "$year-01-01 +$(( $jday - 1 ))days" +%m`
export set DOM=`date -d "$year-01-01 +$(( $jday - 1 ))days" +%d`
export set prevmo=`date -d "$year-01-01 +$(( $jday - 2 ))days" +%m`
export set prevDOM=`date -d "$year-01-01 +$(( $jday - 2 ))days" +%d`

currHMS () { date -d "${year}-${mo}-${DOM} +$(( $min*60 ))seconds" +%H%M%S; }
currHMS_1 () { date -d "${year}-${mo}-${DOM} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }

prevHMS () { date -d "${prevyear}-${prevmo}-${prevDOM} +$(( $min*60 ))seconds" +%H%M%S; }
prevHMS_1 () { date -d "${prevyear}-${prevmo}-${prevDOM} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }


cd /
for (( min = 0; min <= 1410; min = min + 30 ))
do    
    export set dataset="${DATA_PATH}/GLOBAL/IMERG/${year}/LATE/half_hourly/3B-HHR-L.MS.MRG.3IMERG.${year}${mo}${DOM}-S$(currHMS)-E$(currHMS_1).$(printf "%.4d" $min).V03E.RT-H5"
    
    if [ -e "$dataset" ] 
    then
		grads -bcpx "run ${SCRIPTS_PATH}/IMERGsingle_IDN.gs $dataset"
		cp "${dataset}.cal.dat" "$DATA_PATH/IDN/IMERG/$year/RAW/IMERG30m_early.IDN.${year}${mo}${DOM}.$(printf "%.4d" $min).bin"
		cp "${dataset}.cal.ctl" "$DATA_PATH/IDN/IMERG/$year/RAW/IMERG30m_early.IDN.${year}${mo}${DOM}.$(printf "%.4d" $min).ctl"
		cp "$DATA_PATH/IDN/gtifheader/IMERG_IDN.hdr" "$DATA_PATH/IDN/IMERG/$year/RAW/IMERG30m_early.IDN.${year}${mo}${DOM}.$(printf "%.4d" $min).hdr"
		gdalwarp -ts 1031 406 -r near -of EHdr -ot Float32 -srcnodata "-9999.9" -dstnodata "0.0" -overwrite ${DATA_PATH}/IDN/IMERG/${year}/RAW/IMERG30m_early.IDN.${year}${mo}${DOM}.$(printf "%.4d" $min).bin ${DATA_PATH}/IDN/IMERG/${year}/half_hourly/IMERG30m_early.IDN.${year}${mo}${DOM}.$(printf "%.4d" $min).bin
    fi
done

cd "$DATA_PATH/IDN/IMERG/$year/half_hourly"
#Arguments: year startjday endjday timeperiod nrow ncols outputdirectory
dursum_IMERGHDF_IDN $year $jday $jday day 406 1031 "$DATA_PATH/IDN/IMERG/$year/daysum/"
dursum_IMERGHDF_IDN $year $jday $jday p1 406 1031 "$DATA_PATH/IDN/IMERG/$year/15_hrdur/"
dursum_IMERGHDF_IDN $year $jday $jday p2 406 1031 "$DATA_PATH/IDN/IMERG/$year/09_hrdur/"

cd "$HOME"
exit 0
