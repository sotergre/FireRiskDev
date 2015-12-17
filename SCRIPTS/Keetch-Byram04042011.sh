
#last edited 06/25/2009
#inputs args are 1-year(YYYY) 2-julian day(JJJ) 3-previous day's year (YYYY) 4-previous day's julian day(JJJ) 5-enterdata base flag 0=no 1=yes
year=$1
jday=$2
prevyear=$3
prevjday=$4
#enterdata base flag 0=no, 1= just bin, 2= bin & png 
enterdatabase=$5
#################################################################
#		User Defined Parameters				#
#################################################################


npixels=246420 


inputLSTdirectory="/FIRERISK/DATA/AMZ/MODOUT/SFT/"

inputCRUdirectory="/FIRERISK/DATA/AMZ/CRU/"

trmmLRinputdir="/FIRERISK/DATA/AMZ/3B42RT/"

outputdirectory="/FIRERISK/DATA/AMZ/KBDI/"

#########  end user defined paramters	###########


##initialize previous KBDI
#filesnames of last calcuated KBDI file are stored in text files in /FIRERISK/SCRIPTS/
prevKBDI=`cat /FIRERISK/SCRIPTS/KBDI_last.txt`
################################

									
	echo ""
	echo "Calculating Keetch-Byram Index"
	echo ""

	/FIRERISK/BIN/Keetch_Byram_SI ${inputLSTdirectory}${year}/SFT_MOD07_${year}$(printf "%.3d" $jday)_Day_AMZ_int1.bin ${inputCRUdirectory}cru_aveann_ppt_mm_sin.bin ${trmmLRinputdir}${year}/daysum/ppt_3b42rt_${year}$(printf "%.3d" $jday)_sum_AMZ.bin ${prevKBDI} ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ.bin ${outputdirectory}${year}/Delta_KBDI_${year}$(printf "%.3d" $jday)_day_AMZ.bin 246420 

	if [ -f ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ.bin ]
	then
	/FIRERISK/BIN/convert_sint2uchar ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ.bin ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.bin ${npixels} 0 203
	convert -depth 8 -size 555x444 gray:${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.bin ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png
	convert ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png /FIRERISK/SCRIPTS/CLUT/gradient_dslr.png -clut ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png
	convert /FIRERISK/DATA/AMZ/MASK/elv_mask_negate.png ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png -compose Screen  -composite  masked.png
	convert /FIRERISK/DATA/AMZ/MASK/amz_land-elv_mask_negate.png masked.png -compose Multiply  -composite  -fill white -opaque black  ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png
	convert /FIRERISK/DATA/AMZ/MASK/countries.png ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png -compose Screen -composite ${outputdirectory}${year}//KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png
	rm masked.png
	convert ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png /FIRERISK/SCRIPTS/CLUT/KBDImm_vert_right_2.png -background white  -flip +append -flip  ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png

	#overwrite last KBDI file in KBDI_last.txt
	echo "${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ.bin" > /FIRERISK/SCRIPTS/KBDI_last.txt

	else
	cp /FIRERISK/DATA/AMZ/MASK/missing_file.png  ${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png
	fi

	### add to data base ###
	if [ "$enterdatabase" -eq 2 ]
		then
		#save image to database
		saveimagedb.pl -p "${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ_uchar.png" -t 4 -f 0 -r 0 -j "${year}$(printf "%.3d" $jday)"
		saveimagedb.pl -p "${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ.bin" -t 4 -f 1 -r 0 -j "${year}$(printf "%.3d" $jday)"
		elif [ "$enterdatabase" -eq 1 ]
		then
		#only enter binary image into image database
		saveimagedb.pl -p "${outputdirectory}${year}/KB_${year}$(printf "%.3d" $jday)_day_AMZ.bin" -t 4 -f 1 -r 0 -j "${year}$(printf "%.3d" $jday)"
	fi
       echo "PROCESS FINISHED"