#!/bin/bash
export set year=2015
export set jday=91
while [ $jday -lt 286 ]
do
	export set mo=`date -d "$year-01-01 +$(( $jday - 1 ))days" +%m`
	export set DOM=`date -d "$year-01-01 +$(( $jday - 1 ))days" +%d`
	export set prevmo=`date -d "$year-01-01 +$(( $jday - 2 ))days" +%m`
	export set prevDOM=`date -d "$year-01-01 +$(( $jday - 2 ))days" +%d`

	currHMS () { date -d "${year}-${mo}-${DOM} +$(( $min*60 ))seconds" +%H%M%S; }
	currHMS_1 () { date -d "${year}-${mo}-${DOM} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }

	prevHMS () { date -d "${prevyear}-${prevmo}-${prevDOM} +$(( $min*60 ))seconds" +%H%M%S; }
	prevHMS_1 () { date -d "${prevyear}-${prevmo}-${prevDOM} +$(( (min+30)*60 - 1 ))seconds" +%H%M%S; }

	for (( min = 0; min <= 1410; min = min + 30 ))
	do    
		export set dataset="${DATA_PATH}/GLOBAL/IMERG/${year}/half_hourly/3B-HHR-E.MS.MRG.3IMERG.${year}${mo}${DOM}-S$(currHMS)-E$(currHMS_1).$(printf "%.4d" $min).V03E.RT-H5"
		#ctl_Flag=`$SCRIPTS_PATH/ctl.sh`
		if [ -e "$dataset" ] 
		then
			grads -bcpx "run ${SCRIPTS_PATH}/GLOBALIMERG.gs $dataset"
			mv "${dataset}.hqp.dat" "$DATA_PATH/GLOBAL/IMERG/$year/RAW/IMERG30m_early.AMZ.${year}${mo}${DOM}.$(printf "%.4d" $min).bin"
			mv "${dataset}.hqp.ctl" "$DATA_PATH/GLOBAL/IMERG/$year/RAW/IMERG30m_early.AMZ.${year}${mo}${DOM}.$(printf "%.4d" $min).ctl"
			exit 0
			cp "$DATA_PATH/gtifheader/GLOBALIMERG.hdr" "$DATA_PATH/GLOBAL/IMERG/2015/DSLR/$(printf "%.3d" $jday)_Global_DSLR.hdr"
			gdalwarp -s_srs EPSG:3786 -t_srs "+proj=sinu +lon_0=-68.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -ts 622 911 -r near -of EHdr -ot Float32 -overwrite ${DATA_PATH}/AMZEX/IMERG/${year}/RAW/IMERG30m_early.AMZ.${year}${mo}${DOM}.$(printf "%.4d" $min).bin ${DATA_PATH}/GLOBAL/IMERG/${year}/DSLR/IMERG30m_early.AMZ.${year}${mo}${DOM}.$(printf "%.4d" $min).bin
		fi
	done
	jday=$jday + 1
done
exit 0
