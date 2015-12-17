
#last edited 04/13/2009
#################################################################
#		User Defined Parameters				#
#################################################################

enterdatabase=2

npixels=246420 
ncols=444
nrows=555

year=2013

beginjday=188

endjday=188

prevyear=2013

prevjday=187

inputLSTdirectory="/FIRERISK/DATA/AMZ/MODOUT/SFT/"

inputCRUdirectory="/FIRERISK/DATA/AMZ/CRU/"

trmmLRinputdir="/FIRERISK/DATA/AMZ/3B42RT/"

outputdirectory="/FIRERISK/DATA/AMZ/KBDI/"

#########  end user defined paramters	###########

if [ ! -d "/FIRERISK/DATA/AMZ/KBDI/${year}" ]
then
    echo "\nERROR: ${year} is not a directory..Making ${year} one!"
    mkdir /FIRERISK/DATA/AMZ/KBDI/${year}
fi
################################

									
date
### loops through Julian days ###
for (( jday = $beginjday; jday <= $endjday; jday++ ))
	do

	#interpolate	#######
	
	#fmodel
	echo ""
	echo "Calculating Keetch-Byram Index"
	echo ""
	/FIRERISK/BIN/Keetch_Byram_SI ${inputLSTdirectory}${year}/SFT_MOD07_${year}$(printf "%.3d" $jday)_Day_AMZ_int1.bin ${inputCRUdirectory}cru_aveann_ppt_mm_sin.bin ${trmmLRinputdir}${year}/daysum/ppt_3b42rt_${year}$(printf "%.3d" $jday)_sum_AMZ.bin ${outputdirectory}${prevyear}/KB_${prevyear}$(printf "%.3d" $prevjday)_day_AMZ.bin ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ.bin ${outputdirectory}${year}/Delta_KBDI_${year}$(printf "%.3d" $jday)_day_AMZ.bin 246420 
	#The second ouput is for debugging, Delta_KBDI is commented out in Keetch_Byram_SI code.
	if [ -s ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ.bin ]
		then
		prevyear=${year}
		prevjday=${jday}	
		echo "prevdate is ${prevyear} ${prevjday}"
		fi

	if [ -f ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ.bin ]
	then
	echo "here"
	/FIRERISK/BIN/convert_sint2uchar ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ.bin ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.bin ${npixels} 0 203
	convert -depth 8 -size 555x444 gray:${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.bin ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png
	convert ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png /FIRERISK/SCRIPTS/CLUT/gradient_dslr.png -clut ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png
	convert /FIRERISK/DATA/AMZ/MASK/elv_mask_negate.png ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png -compose Screen  -composite  masked.png
	convert /FIRERISK/DATA/AMZ/MASK/amz_land-elv_mask_negate.png masked.png -compose Multiply  -composite  -fill white -opaque black  ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png
	convert /FIRERISK/DATA/AMZ/MASK/countries.png ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png -compose Screen -composite ${outputdirectory}${year}//KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png
	rm masked.png
	convert ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png /FIRERISK/SCRIPTS/CLUT/KBDImm_vert_right_2.png -background white  -flip +append -flip  ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png
	else
	cp /FIRERISK/DATA/AMZ/MASK/missing_file.png  ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png
	fi

	
	cd ${outputdirectory}${year}/
		
	###  write header file  ###
	echo "ncols $ncols" > KB_${year}$(printf "%.3d" $jday)_day_AMZ.hdr
	echo "nrows $nrows" >> KB_${year}$(printf "%.3d" $jday)_day_AMZ.hdr
	echo "nbands 1" >> KB_${year}$(printf "%.3d" $jday)_day_AMZ.hdr
	echo "nbits 16" >> KB_${year}$(printf "%.3d" $jday)_day_AMZ.hdr
	echo "cellsize 5000" >> KB_${year}$(printf "%.3d" $jday)_day_AMZ.hdr
	echo "xllcenter -1665425.7796280" >> KB_${year}$(printf "%.3d" $jday)_day_AMZ.hdr
	echo "yllcenter -2217500.0" >> KB_${year}$(printf "%.3d" $jday)_day_AMZ.hdr
	echo "nodata_value 0" >> KB_${year}$(printf "%.3d" $jday)_day_AMZ.hdr
	echo "pixeltype SIGNEDINT" >> KB_${year}$(printf "%.3d" $jday)_day_AMZ.hdr
	echo "byteorder I" >> KB_${year}$(printf "%.3d" $jday)_day_AMZ.hdr

	###  convert to geotiff  ###
	echo "gdal starts here"
	#convert binary to geotiff
	gdal_translate -ot Int16 -of GTiff KB_${year}$(printf "%.3d" $jday)_day_AMZ.bin KB_${year}$(printf "%.3d" $jday)_day_AMZ_temp.tif

	gdalwarp -s_srs "+proj=sinu +lon_0=-60.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=-60.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite KB_${year}$(printf "%.3d" $jday)_day_AMZ_temp.tif KB_${year}$(printf "%.3d" $jday)_day_AMZ.tif
	rm KB_${year}$(printf "%.3d" $jday)_day_AMZ_temp.tif
		
	cd ${SCRIPTS_PATH}




### add to data base ###
	if [ "$enterdatabase" -eq 2 ]
		then
		#save image to database
		saveimagedb.pl -p "${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png" -t 4 -f 0 -r 0 -j "${year}$(printf "%.3d" $jday)"
		saveimagedb.pl -p "${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ.bin" -t 4 -f 1 -r 0 -j "${year}$(printf "%.3d" $jday)"
		saveimagedb.pl -p "${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ.tif" -t 4 -f 2 -r 0 -j "${year}$(printf "%.3d" $jday)"

		elif [ "$enterdatabase" -eq 1 ]
		then
		#only enter binary image into image database
		saveimagedb.pl -p "${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ.bin" -t 4 -f 1 -r 0 -j "${year}$(printf "%.3d" $jday)"
	fi

	done
		
	date
        echo "PROCESS FINISHED"