#!/bin/bash

printf "\nMoving into $DATA_PATH/GLOBAL/IMERG/$year/half_hourly\n"
cd "$DATA_PATH/GLOBAL/IMERG/$year/half_hourly"
if [ "$jday" -gt 72 ]
then
    export set ver="V04A"
else
    export set ver="V03E"
fi

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
    export set dataset="${DATA_PATH}/GLOBAL/IMERG/${year}/half_hourly/3B-HHR-E.MS.MRG.3IMERG.${year}${mo}${DOM}-S$(currHMS)-E$(currHMS_1).$(printf "%.4d" $min).$ver.RT-H5"
    #ctl_Flag=`$SCRIPTS_PATH/ctl.sh`
    if [ -e "$dataset" ] 
    then
		grads -bcpx "run ${SCRIPTS_PATH}/IMERGsingle.gs $dataset"
		mv "${dataset}.hqp.dat" "$DATA_PATH/AMZEX/IMERG/$year/RAW/IMERG30m_early.AMZ.${year}${mo}${DOM}.$(printf "%.4d" $min).bin"
		mv "${dataset}.hqp.ctl" "$DATA_PATH/AMZEX/IMERG/$year/RAW/IMERG30m_early.AMZ.${year}${mo}${DOM}.$(printf "%.4d" $min).ctl"
		cp "$DATA_PATH/gtifheader/IMERG.hdr" "$DATA_PATH/AMZEX/IMERG/$year/RAW/IMERG30m_early.AMZ.${year}${mo}${DOM}.$(printf "%.4d" $min).hdr"
		gdalwarp -s_srs EPSG:3786 -t_srs "+proj=sinu +lon_0=-68.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -ts 622 911 -r near -of EHdr -ot Float32 -overwrite ${DATA_PATH}/AMZEX/IMERG/${year}/RAW/IMERG30m_early.AMZ.${year}${mo}${DOM}.$(printf "%.4d" $min).bin ${DATA_PATH}/AMZEX/IMERG/${year}/half_hourly/IMERG30m_early.AMZ.${year}${mo}${DOM}.$(printf "%.4d" $min).bin
    fi
done

cd "$DATA_PATH/AMZEX/IMERG/$year/half_hourly"
#Arguments: year startjday endjday timeperiod nrow ncols outputdirectory
dursum_IMERG_AMZ_GMT $year $jday $jday day 911 622 "$DATA_PATH/AMZEX/IMERG/$year/daysum/"
dursum_IMERG_AMZ_GMT $year $jday $jday p1 911 622 "$DATA_PATH/AMZEX/IMERG/$year/15_hrdur/"
dursum_IMERG_AMZ_GMT $year $jday $jday p2 911 622 "$DATA_PATH/AMZEX/IMERG/$year/09_hrdur/"

cd "$HOME"
exit 0
