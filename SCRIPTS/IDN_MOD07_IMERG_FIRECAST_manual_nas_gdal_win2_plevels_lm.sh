#!/bin/bash

#This script shuld be maintained as an exact copy of IDN_MOD07_FIRERISK.sh with the
#exception of lines 85-87 and is used for manual runs. This script is initated with firerisk_init_manual.sh
#This script uses previous day's dates instead of reading the 
#last_LM0010.txt, last_LM0100.txt, last_LM1000.txt files to get the previous day's filenames.
#inputs args are 1-year(YYYY) 2-julian day(JJJ) 3-previous day's year (YYYY) 4-previous day's julian day(JJJ) 5-enterdata base flag 0=no 1=yes

###############################################################
#		User Defined Parameters				#
###############################################################

#prefix is either MOD or MYD
prefix=MOD 
#pass is either Day or Night
pass=Day
######################
#   FLAGS		#
#if 1 then HDF files are processed from HDF and reprojected
#change to 0 if the binaries already exist
process=1
#RH flag - 1 calc RH on | 0 calc RH off
calcRH=1
#interpolate flag - 1 interpolate on | 0 interpolate off
interpolate=1
#calculate litter moisture flag - 1 on | 0 off
moisture=1
#####################
###   set geographic parameters in DD

UL_X=94.5
UL_Y=7.5
LR_X=143.5
LR_Y=-11.0


####pixel size 1000m=0.008333 and 5000m=0.041665
pixel_size_DD=0.041665
pixel_size_m=5000
#resulting pixel size ill be 5010 in X direction and 5005 in the Y direction becuase of the HDFLook output. For the geotiff image we will use a resulting pixel size of 5007 meters

####nrows = ABS( (UL_X-LR_X)/ pixel_size_DD )
nrows=414
####ncolss = ABS( (UL_X-LR_X)/ pixel_size_DD )
ncols=1089
#nrows*ncols
npixels=$(( $nrows*$ncols ))

### TRMM Pixel Count for Historical Runs before 05-01-2015
#nrowsTRMM=165
#ncolsTRMM=113
#npixelsTRMM=$(( $nrowsTRMM*$ncolsTRMM ))

echo "IDN:"
#####################################
########set directories#############
cd ~
####executable directory###
#HOME="/firecast"
echo "HOME:$HOME"
export set home_dir="$HOME/BIN"
echo "home_dir:$home_dir"
#DATA_PATH="$HOME/DATA"
echo "DATA_PATH:$DATA_PATH"
export set HDFbinDIR="/opt/local/bin"


####mask name and location###
export set mask="${DATA_PATH}/IDN/Mask/IDN_Mask_Merc.bin"
if [ ! -e "$mask" ]
then
	echo "30: $mask not found"
    exit 30
fi

###MOD07 input directory####
MOD07_indir="${DATA_PATH}/IDN/${prefix}07L2/"
if [ ! -e "$MOD07_indir" ] 
then
	echo "40 $MOD07_indir not found"
    exit 40
fi

##### Root Out directory   ####
outputdirectory="${DATA_PATH}/IDN/${prefix}OUT/"
if [ ! -e "$outputdirectory" ] 
then
    mkdir -p "$outputdirectory"
    echo "Created: $outputdirectory"
fi

##initialize previous day's files
#filesnames of last calcuated litter moisture files are stored in text files in $SCRIPTS_PATH/
if [[ $jday == 1 ]]
then
    prevLMfile10="${DATA_PATH}/IDN/MODOUT/LM/2015/HR0010/LM0010h_2015365_Day_IDN.bin"
    prevLMfile100="${DATA_PATH}/IDN/MODOUT/LM/2015/HR0100/LM0100h_2015365_Day_IDN.bin"
    prevLMfile1000="${DATA_PATH}/IDN/MODOUT/LM/2015/HR1000/LM1000h_2015365_Day_IDN.bin"
else
    prevLMfile10="${DATA_PATH}/IDN/MODOUT/LM/${prevyear}/HR0010/LM0010h_${prevyear}$(printf "%.3d" $(( $jday-1 )))_Day_IDN.bin"
    prevLMfile100="${DATA_PATH}/IDN/MODOUT/LM/${prevyear}/HR0100/LM0100h_${prevyear}$(printf "%.3d" $(( $jday-1 )))_Day_IDN.bin"
    prevLMfile1000="${DATA_PATH}/IDN/MODOUT/LM/${prevyear}/HR1000/LM1000h_${prevyear}$(printf "%.3d" $(( $jday-1 )))_Day_IDN.bin"
fi
# if [ ! -e "$prevLMfile10" ] 
# then
#     exit 155
# fi
# if [ ! -e "$prevLMfile100" ] 
# then
#     exit 156
# fi
# if [ ! -e "$prevLMfile1000" ]
# then
#     exit 157
# fi

###############   TRMM         #################
####	rainfall input directory 		#######
imerginputdir="${DATA_PATH}/IDN/IMERG/$year/half_hourly/"
if [ ! -e "$imerginputdir" ] 
then
    mkdir -p "$imerginputdir"
    echo "Created: $imerginputdir"
fi





#########		end user defined paramters	###########
###############################################################						
	
	#####################generate batch files for HDFLook and execute ###################
	##########	extracts surface pressure from MOD07	#####################
	##########	Projects surface pressure to Sinusoidal			#####################
echo "Enter Process"
	if [ "$process" -eq 1 ]
	then
	    cd ${DATA_PATH}/IDN/MODOUT
		########		generate batch file for Surface Pressure		###############
		echo "verbose" > ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch
		echo "clear_data" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch
		echo "" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch
		echo "set_input_directory    ${MOD07_indir}${year}/" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch
		echo "set_output_directory    ${outputdirectory}SF_Pressure/${year}/" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch
		echo "" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch
		echo "set_projection_to_geometry   ProjectionTo=\"Sinusoidal\" WidthTo=1089 HeightTo=414 PixelSizeXTo=$pixel_size_m PixelSizeYTo=$pixel_size_m LatitudeMinTo=$LR_Y LatitudeMaxTo=$UL_Y LongitudeMinTo=$UL_X LongitudeMaxTo=$LR_X CenterLongitudeTo=119.0"  >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch
		echo "set_input_hdf_file         ${MOD07_indir}${year}/${prefix}07_L2.A${year}$(printf "%.3d" $jday)*" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch 
		echo "select_SDS                 SDSName= \"Surface_Pressure\" Index=1 " >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch
		echo "set_misc_options Only${pass}=Yes" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch
		echo "create_MODIS_SDS_MOSAIC    FileName=\"SP_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin\" ClearFile=Yes scaling=\"y=a(x-b)\" " >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch
		#echo "create_MODIS_SDS_MOSAIC    FileName=\"SP_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin\" ClearFile=Yes " >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch
	
		#run HDFLook
		${HDFbinDIR}/HDFLook ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch
		mv ${outputdirectory}SF_Pressure/${year}/SP_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ${outputdirectory}SF_Pressure/${year}/SP_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin
		rm ${outputdirectory}SF_Pressure/${year}/${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch_STATUS
		rm ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SP.batch

		if [ ! -e "${outputdirectory}SF_Pressure/${year}/" ]
		then
			mkdir -p ${outputdirectory}SF_Pressure/${year}/
		fi
		cd ${outputdirectory}SF_Pressure/${year}/
	
		###  write header file  ###
		cp $DATA_PATH/IDN/gtifheader/FLOAT.hdr SP_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.hdr

		###  convert to geotiff  ###
		#echo "gdal starts here"
		#convert binary to geotiff
		#gdal_translate -ot Int32 -of GTiff SP_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin SP_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif

		#gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite SP_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif SP_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.tif
		#rm SP_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif
		
		cd ${SCRIPTS_PATH}





		#####################generate batch files for HDFLook and execute ###################
		##########	extracts AT and DPT from Pressure Profiles 4-20	#####################
		##########	Projects AT and DPT to Sinusoidal			#####################

		for profile in 13 14 15 16 17 18 19 20
			do
			########		generate batch file for Dew Point Profile		###############
			echo "verbose" > ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch
			echo "clear_data" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch
			echo "" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch
			echo "set_input_directory    ${MOD07_indir}${year}/" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch
			echo "set_output_directory    ${outputdirectory}TP_PROFILE/${year}/" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch
			echo "" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch
			echo "set_projection_to_geometry   ProjectionTo=\"Sinusoidal\" WidthTo=1089 HeightTo=414 PixelSizeXTo=$pixel_size_m PixelSizeYTo=$pixel_size_m LatitudeMinTo=$LR_Y LatitudeMaxTo=$UL_Y LongitudeMinTo=$UL_X LongitudeMaxTo=$LR_X CenterLongitudeTo=119.0 "  >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch
			echo "set_input_hdf_file         ${MOD07_indir}${year}/${prefix}07_L2.A${year}$(printf "%.3d" $jday)*" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch 
			echo "select_SDS                 SDSName= \"Retrieved_Moisture_Profile\" Index=${profile} " >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch
			echo "set_misc_options Only${pass}=Yes" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch
			echo "create_MODIS_SDS_MOSAIC    FileName=\"DPT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin\" ClearFile=Yes scaling=\"y=a(x-b)\" Index=${profile}" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch

			#run HDFLook
			${HDFbinDIR}/HDFLook ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch
			mv ${outputdirectory}TP_PROFILE/${year}/DPT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin_${profile} ${outputdirectory}TP_PROFILE/${year}/DPT_${profile}_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin
			rm ${outputdirectory}TP_PROFILE/${year}/${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch_STATUS
			rm ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2DPT.batch

			if [ ! -e "${outputdirectory}TP_PROFILE/${year}/" ]
			then
				mkdir -p ${outputdirectory}TP_PROFILE/${year}/
			fi
			cd ${outputdirectory}TP_PROFILE/${year}/

			###  write header file  ###
			cp /$DATA_PATH/IDN/gtifheader/FLOAT.hdr DPT_${profile}_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.hdr
		
			###  convert to geotiff  ###
			#echo "gdal starts here"
			#convert binary to geotiff
			#gdal_translate -ot Int32 -of GTiff DPT_${profile}_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin DPT_${profile}_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif

			#gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite DPT_${profile}_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif DPT_${profile}_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.tif
			#rm DPT_${profile}_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif

			cd ${SCRIPTS_PATH}
	
	
		########		generate batch file for Atmospheric Temperature profile 	###############
		echo "verbose" > ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch
		echo "clear_data" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch
		echo "" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch
		echo "set_input_directory    ${MOD07_indir}${year}/" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch
		echo "set_output_directory    ${outputdirectory}TP_PROFILE/${year}/" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch
		echo "" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch
		echo "set_projection_to_geometry   ProjectionTo=\"Sinusoidal\"  CenterLongitudeTo=0.0 WidthTo=1089 HeightTo=414 PixelSizeXTo=$pixel_size_m PixelSizeYTo=$pixel_size_m LatitudeMinTo=$LR_Y LatitudeMaxTo=$UL_Y LongitudeMinTo=$UL_X LongitudeMaxTo=$LR_X " >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch
		echo "set_input_hdf_file         ${MOD07_indir}${year}/${prefix}07_L2.A${year}$(printf "%.3d" $jday)*" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch
		echo "select_SDS                 SDSName= \"Retrieved_Temperature_Profile\" Index=${profile}" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch
		echo "set_misc_options Only${pass}=Yes" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch
		echo "create_MODIS_SDS_MOSAIC    FileName=\"AT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin\" ClearFile=Yes scaling=\"y=a(x-b)\" Index=${profile}" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch
	
		###  execute AT script  ###
		${HDFbinDIR}/HDFLook ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch
		mv ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin_${profile} ${outputdirectory}TP_PROFILE/${year}/AT_${profile}_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin
		rm ${outputdirectory}TP_PROFILE/${year}/${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch_STATUS
		rm ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2AT.batch

		if [ ! -e "${outputdirectory}TP_PROFILE/${year}/" ]
		then
			mkdir -p ${outputdirectory}TP_PROFILE/${year}/
		fi
		cd ${outputdirectory}TP_PROFILE/${year}/
	
		###  write header file  ###
		cp /$DATA_PATH/IDN/gtifheader/FLOAT.hdr AT_${profile}_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.hdr
	
		###  convert to geotiff  ###
		#echo "gdal starts here"
		#convert binary to geotiff
		#gdal_translate -ot Int32 -of GTiff AT_${profile}_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin AT_${profile}_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif

		#gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite AT_${profile}_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif AT_${profile}_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.tif
		#rm AT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif
		
		cd ${SCRIPTS_PATH}
	
		#end levels loops
		done

		########	generate batch file for MODIS 07L2 Surface Temperature ###############

		echo "verbose" > ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch
		echo "clear_data" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch
		echo "" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch
		echo "set_input_directory    ${MOD07_indir}${year}/" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch
		echo "set_output_directory   ${outputdirectory}SFT/${year}/" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch
		echo "" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch	
		echo 'set_projection_to_geometry   ProjectionTo='Sinusoidal'  CenterLongitudeTo=0.0 WidthTo=1089 HeightTo=414 PixelSizeXTo='$pixel_size_m' PixelSizeYTo='$pixel_size_m' LatitudeMinTo='$LR_Y' LatitudeMaxTo='$UL_Y' LongitudeMinTo='$UL_X' LongitudeMaxTo='$LR_X' '  >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch	
		echo "set_input_hdf_file         ${MOD07_indir}${year}/${prefix}07_L2.A${year}$(printf "%.3d" $jday)*" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch
		echo "select_SDS                 SDSName= \"Skin_Temperature\" " >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch
		#echo "create_MODIS_SDS_MOSAIC    FileName=\"SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin\" ClearFile=Yes CreateGEOTIFF=yes CreateJPEG=yes scaling=\"y=a(x-b)\" Index=1" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch
		echo "create_MODIS_SDS_MOSAIC    FileName=\"SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin\" ClearFile=Yes scaling=\"y=a(x-b)\" Index=1" >> ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch

	
		###  execute SFT script  ###
		${HDFbinDIR}/HDFLook  ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch
		rm ${outputdirectory}SFT/${year}/${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch_STATUS
		rm ${prefix}07${year}$(printf "%.3d" $jday)MOD07L2SFT.batch
	
		if [ ! -e "${outputdirectory}SFT/${year}/" ]
		then
			mkdir -p ${outputdirectory}SFT/${year}/
		fi
		cd ${outputdirectory}SFT/${year}/
	
		###  write header file  ###
		cp /$DATA_PATH/IDN/gtifheader/FLOAT.hdr SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.hdr
		
	
		###  convert to geotiff  ###
		#echo "gdal starts here"
		#convert binary to geotiff
		#gdal_translate -ot Float32 -of GTiff SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif
	
		#gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.tif

		#gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -te "+xmin=-1516831.82 +ymin=-3113461.46 +xmax=1374511.65 +ymax=1445535.68" -tr "+xres=5000 +yres=5000" -ts "+width=579 +height=912" -of GenBin -overwrite SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_2.bin
		#rm SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif
	
		cd ${SCRIPTS_PATH}
	
		echo "****************************** "
		echo "Finished extracting HDF data"
		echo "****************************** "

	fi

	##########		   		############
	##########	relative humidity	############
	##########		   		############	
	if [ "$calcRH" -eq 1 ]
	then
		echo ""
		echo "Calculating Relative Humidity"
		echo ""
        
        #Arguments:<julian day> <year> <output directory> <image size in total pixels>
        #32 bit FLOAT output
		${home_dir}/rhumid_st_dpt_nas_plevels_idn $jday $year $outputdirectory $npixels
		
		if [ ! -e "${outputdirectory}RH/${year}/" ]
		then
			mkdir -p ${outputdirectory}RH/${year}/
		fi
		cd ${outputdirectory}RH/${year}/
	
		###  write header file  ###
		cp /$DATA_PATH/IDN/gtifheader/FLOAT.hdr RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.hdr
		
	
		###  convert to geotiff  ###
		echo "gdal starts here"
		#convert binary to geotiff
		#gdal_translate -ot Int16 -of GTiff RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif

		gdal_translate -ot Float32 -of GTiff RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif
		#gdal_translate -ot Float32 -of EHdr  RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN32.bin
		
		gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.tif

	
		rm RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif
	
		cd ${SCRIPTS_PATH}	

	fi

	if [ "$interpolate" -eq 1 ]
	then
	######	interpolate	#####
		if [ -f ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ]
		then

			${home_dir}/interpolate_RH_temporal_nas_32day ${mask} ${outputdirectory}RH/ ${outputdirectory}RH/ ${year} ${jday} ${ncols} ${nrows} -1
		
			if [ ! -e "${outputdirectory}RH/${year}/" ]
			then
				mkdir -p ${outputdirectory}RH/${year}/
			fi
			cd ${outputdirectory}RH/${year}/
	
			###  write header file for RH_interp ###
			cp /$DATA_PATH/IDN/gtifheader/SIGNED16.hdr RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp.hdr
			cp /$DATA_PATH/IDN/gtifheader/MaskProj.prj RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp.prj
	
	
			###  convert to geotiff  ###
			echo "gdal starts here"
			#convert binary to geotiff
	
			gdal_translate -ot Int16 -of GTiff RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp.bin RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_temp.tif
	
			gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_temp.tif RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp.tif

	
			rm RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_temp.tif

			###  write header file for RH_interp_count ###
			echo "ncols 1089" > RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "nrows 414" >> RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "nbands 1" >> RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "nbits 8" >> RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "cellsize 5007" >> RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "ULXMAP 10517709.15721" >> RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "ULYMAP 838258.46158389" >> RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "nodata_value 0" >> RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			#echo "pixeltype Byte" >> RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			#echo "byteorder I" >> RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr		
	
			###  convert to geotiff  ###
			echo "gdal starts here"
			#convert binary to geotiff
	
			gdal_translate -ot Byte -of GTiff RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.bin RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count_temp.tif
	
			gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count_temp.tif RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.tif

	
			rm RH_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count_temp.tif
	
			cd ${SCRIPTS_PATH}	

		 
		fi
	
		if [ -f ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ]
		then
			#SFT exists. change flag in SFT_retry to 0.
			echo "0" > $SCRIPTS_PATH/SFT_retry.txt
			${home_dir}/interpolate_SFT_temporal_nas_32day ${mask} ${outputdirectory}SFT/ ${outputdirectory}SFT/ ${year} ${jday} ${ncols} ${nrows} -1
			cd ${outputdirectory}SFT/${year}/
	
			###  write header file for SFT_interp ###
			cp /$DATA_PATH/IDN/gtifheader/SIGNED16.hdr SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp.hdr
			cp /$DATA_PATH/IDN/gtifheader/MaskProj.prj SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp.prj
	
	
			###  convert to geotiff  ###
			echo "gdal starts here"
			#convert binary to geotiff
	
			gdal_translate -ot Int16 -of GTiff SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp.bin SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_temp.tif
	
			gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_temp.tif SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp.tif

	
			rm SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_temp.tif

			###  write header file for SFT_interp_count ###
			echo "ncols $ncols" > SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "nrows $nrows" >> SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "nbands 1" >> SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "nbits 8" >> SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "cellsize 5007" >> SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "ULXMAP 10517709.15721" >> SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "ULYMAP 838258.46158389" >> SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			echo "NODATA 0" >> SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			#echo "pixeltype Byte" >> SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr
			#echo "byteorder I" >> SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.hdr		
	
			###  convert to geotiff  ###
			echo "gdal starts here"
			#convert binary to geotiff
	
			gdal_translate -ot Byte -of GTiff SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.bin SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count_temp.tif
	
			gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count_temp.tif SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count.tif

	
			rm SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_interp_count_temp.tif
	
			cd ${SCRIPTS_PATH}	
		else
			#SFT does not exist. Change flag in SFT_retry to 1. Firerisk_init will try to download MOD07 for this day before running the next day.
			echo "1" > $SCRIPTS_PATH/SFT_retry.txt
			#copy yesterday's SFT to today's file (new 06-27-2009)
			#cp ${outputdirectory}SFT/${prevyear}/SFT_${prefix}07_${prevyear}$(printf "%.3d" $(( $jday-1 )))_${pass}_IDN_int1.bin  ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_int1.bin
			#cp ${outputdirectory}SFT/${prevyear}/SFT_${prefix}07_${prevyear}$(printf "%.3d" $(( $jday-1 )))_${pass}_IDN_int1_20to100.png  ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_int1_20to100.png
			#cp ${outputdirectory}SFT/${prevyear}/SFT_${prefix}07_${prevyear}$(printf "%.3d" $(( $jday-1 )))_${pass}_IDN_int1_40.png  ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_int1_40.png
			#cp ${outputdirectory}SFT/${prevyear}/SFT_${prefix}07_${prevyear}$(printf "%.3d" $(( $jday-1 )))_${pass}_IDN_dis1_500km.png  ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $jday)_${pass}_IDN_dis1_500km.png
		fi

	echo "****************************** "
	echo "Finished interpolation"
	echo "****************************** "

	fi





	


	##########		   		############
	##########	moisture content	############
	##########		   		############	

	if [ "$moisture" -eq 1 ]
	then

		#fmodel - calculate soil moisture based on yesterday's rainfall
		echo ""
		echo "Calculating Litter Moisture"
		echo ""

		echo ""
		echo "NOT Calculating 10 HR"
		echo ""
#		${home_dir}/fmodel_10hr_nas_v2_IMERG ${year} $(printf "%.3d" $jday) ${prevyear} $(printf "%.3d" $(( $jday-1 ))) ${npixels} ${DATA_PATH}
# 	
# 		#############create geotiff ###################
# 		if [ ! -e "${outputdirectory}LM/${year}/HR0010" ]
# 		then
# 			mkdir -p ${outputdirectory}LM/${year}/HR0010
# 		fi
 		cd ${outputdirectory}LM/${year}/HR0010
# 	
# 		###  write header file for LM0010 index ###
# 		cp ${DATA_PATH}/IDN/gtifheader/UNSIGNED8.hdr LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN.hdr
# 	
# 		#convert binary to geotiff
# 	
# 		gdal_translate -ot Int16 -of GTiff LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif
# 		gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN.tif
# 		rm LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif
# 
# 	
# 		
# 		##copy hdr for prevlm##
# 		cp ${DATA_PATH}/IDN/gtifheader/UNSIGNED8.hdr LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last.hdr
# 	
# 		gdal_translate -ot Int16 -of GTiff LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last.bin LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last_temp.tif
# 		gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last_temp.tif LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last.tif
# 		rm LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last_temp.tif
# 
# 		###  write header file for LM0010 count ###
# 		cp ${DATA_PATH}/IDN/gtifheader/UNSIGNED8.hdr LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count.hdr
# 		gdal_translate -ot Int16 -of GTiff LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count.bin LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count_temp.tif
# 		gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count_temp.tif LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count.tif
# 		rm LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count_temp.tif
# 	
		cd ${SCRIPTS_PATH}
		##############################################
# 		
# 		if [ -f ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ]
# 		then
# 			#convert LM0010 to png
# 			#${home_dir}/rescale_dslr ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.bin ${npixels} 40
# 			#convert -depth 8 -size 555x444 gray:${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.bin ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
# 			#convert ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png $SCRIPTS_PATH/CLUT/gradient_all.png -elut ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
# 			#convert ${DATA_PATH}/IDN/MASK/savannah_mask.png ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png -eompose Screen -eomposite -fuzz 20000 -fill gray92 -opaque white masked.png
# 			#convert ${DATA_PATH}/IDN/MASK/negate.png masked.png  -eompose Screen -eomposite  masked.png 
# 			#convert ${DATA_PATH}/IDN/MASK/IDN_land-elv_mask_negate.png masked.png  -eompose Multiply  -eomposite -fill white -opaque black  ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
# 			#convert ${DATA_PATH}/IDN/MASK/countries.png ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png -eompose Screen -eomposite ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
# 			#rm masked.png
# 			#overwrite last LM file in LM0010_last.txt
# 			echo "${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin" > $SCRIPTS_PATH/LM0010_last.txt
# 
# 		else
# 			#cp ${DATA_PATH}/IDN/MASK/missing_file.png ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
# 			printf "\n\n\n******Line 556***************************************************\n\n\n"
#         fi
# 				
# 
# 		echo "prevLM10 is file is $prevLMfile10"
# 
		echo ""
		echo "Calculating 100 HR for JDAY => $jday"
		echo ""
		
		if [[ $jday == 1 ]]
		then
		    ${home_dir}/fmodel_100hr_1000hr_nas_v2_IMERG_idn ${year} $(printf "%.3d" $jday) ${prevyear} 365 ${npixels} ${DATA_PATH} 100
		else
		    ${home_dir}/fmodel_100hr_1000hr_nas_v2_IMERG_idn ${year} $(printf "%.3d" $jday) ${prevyear} $(printf "%.3d" $(( $jday-1 ))) ${npixels} ${DATA_PATH} 100
        fi
        
		#############create geotiff ###################
		if [ ! -e "${outputdirectory}LM/${year}/HR0100" ]
		then
			mkdir -p ${outputdirectory}LM/${year}/HR0100
		fi
		cd ${outputdirectory}LM/${year}/HR0100
	
		###  write header file for LM0100 index ###
		cp ${DATA_PATH}/IDN/gtifheader/SIGNED16.hdr LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN.hdr
	
		#convert binary to geotiff
	
		gdal_translate -ot Int16 -of GTiff LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif
		gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN.tif
		rm LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif

	
		
		##copy hdr for prevlm##
		cp ${DATA_PATH}/IDN/gtifheader/SIGNED16.hdr LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last.hdr
	
		gdal_translate -ot Int16 -of GTiff LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last.bin LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last_temp.tif
		gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last_temp.tif LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last.tif
		rm LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last_temp.tif

		###  write header file for LM0100 count ###
		cp ${DATA_PATH}/IDN/gtifheader/SIGNED16.hdr LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count.hdr
		gdal_translate -ot Int16 -of GTiff LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count.bin LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count_temp.tif
		gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count_temp.tif LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count.tif
		rm LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count_temp.tif

	
		cd ${SCRIPTS_PATH}	
		##############################################

			if [ -f ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ]
			then
				#convert LM0100 to png
				#${home_dir}/rescale_dslr ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.bin ${npixels} 40
				#convert -depth 8 -size 555x444 gray:${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.bin ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
				#convert ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png $SCRIPTS_PATH/CLUT/gradient_all.png -elut ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
				#convert ${DATA_PATH}/IDN/MASK/savannah_mask.png ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png -eompose Screen -eomposite -fuzz 20000 -fill gray92 -opaque white masked.png
				#convert ${DATA_PATH}/IDN/MASK/negate.png masked.png  -eompose Screen -eomposite  masked.png 
				#convert ${DATA_PATH}/IDN/MASK/IDN_land-elv_mask_negate.png masked.png  -eompose Multiply  -eomposite -fill white -opaque black  ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
				#convert ${DATA_PATH}/IDN/MASK/countries.png ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png -eompose Screen -eomposite ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
				#rm masked.png
				#overwrite last LM file in LM0100_last.txt
				echo "${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin" > $SCRIPTS_PATH/LM0100_last.txt

			else
				#cp ${DATA_PATH}/IDN/MASK/missing_file.png  ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
			    echo ""
			fi
		echo "prevLMfile100 is $prevLMfile100 "
	
		echo ""
		echo "Calculating 1000 HR"
		echo ""
		
		if [[ $jday == 1 ]]
		then
		    ${home_dir}/fmodel_100hr_1000hr_nas_v2_IMERG_idn ${year} $(printf "%.3d" $jday) ${prevyear} 365 ${npixels} ${DATA_PATH} 1000
		else
		    ${home_dir}/fmodel_100hr_1000hr_nas_v2_IMERG_idn ${year} $(printf "%.3d" $jday) ${prevyear} $(printf "%.3d" $(( $jday-1 ))) ${npixels} ${DATA_PATH} 1000
        fi
        
		#############create geotiff ###################
		if [ ! -e "${outputdirectory}LM/${year}/HR1000" ]
		then
			mkdir -p ${outputdirectory}LM/${year}/HR1000
		fi
		cd ${outputdirectory}LM/${year}/HR1000
	
		###  write header file for LM1000 index ###
		cp ${DATA_PATH}/IDN/gtifheader/SIGNED16.hdr LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN.hdr
	
		#convert binary to geotiff
	
		gdal_translate -ot Int16 -of GTiff LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif
		gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN.tif
		rm LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif

	
		
		##copy hdr for prevlm##
		cp ${DATA_PATH}/IDN/gtifheader/SIGNED16.hdr LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last.hdr
	
		gdal_translate -ot Int16 -of GTiff LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last.bin LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last_temp.tif
		gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last_temp.tif LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last.tif
		rm LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_last_temp.tif

		###  write header file for LM1000 count ###
		cp ${DATA_PATH}/IDN/gtifheader/SIGNED16.hdr LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count.hdr
		gdal_translate -ot Int16 -of GTiff LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count.bin LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count_temp.tif
		gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -of GTiff -overwrite LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count_temp.tif LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count.tif
		rm LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_count_temp.tif

	
		cd ${SCRIPTS_PATH}	
		##############################################

			if [ -f ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ]
			then
				#convert LM1000 to png
				#${home_dir}/rescale_dslr ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.bin ${npixels} 40
				#convert -depth 8 -size 555x444 gray:${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.bin ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
				#convert ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png $SCRIPTS_PATH/CLUT/gradient_all.png -elut ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
				#convert ${DATA_PATH}/IDN/MASK/savannah_mask.png ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png -eompose Screen -eomposite -fuzz 20000 -fill gray92 -opaque white masked.png
				#convert ${DATA_PATH}/IDN/MASK/negate.png masked.png  -eompose Screen -eomposite  masked.png 
				#convert ${DATA_PATH}/IDN/MASK/IDN_land-elv_mask_negate.png masked.png  -eompose Multiply  -eomposite -fill white -opaque black  ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
				#convert ${DATA_PATH}/IDN/MASK/countries.png ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png -eompose Screen -eomposite ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
				#rm masked.png
				#overwrite last LM file in LM1000_last.txt
				echo "${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin" > $SCRIPTS_PATH/LM1000_last.txt

			else
				#cp ${DATA_PATH}/IDN/MASK/missing_file.png  ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
			    echo ""
			fi
		echo "prevLMfile1000 is $prevLMfile1000"

		echo ""
		echo "Calculating Fire Risk Index"
		echo ""
		##########generate fire risk#########
		${home_dir}/firerisk_index_nas_v2 ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ${npixels}

		#############create geotiff ###################
		if [ ! -e "${outputdirectory}FR_INDEX/${year}/" ]
		then
			mkdir -p ${outputdirectory}FR_INDEX/${year}/
		fi
		cd ${outputdirectory}FR_INDEX/${year}/
	
		###  write header file for fire risk index ###
		cp $DATA_PATH/IDN/gtifheader/SIGNED16.hdr FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN.hdr
			
	
		###  convert to geotiff  ###
		echo "gdal starts here"
		#convert binary to geotiff
	
		gdal_translate -ot Int16 -of GTiff FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif
	
		gdalwarp -s_srs "+proj=sinu +lon_0=0.0 +x_0=0 +y_0=0 +datum=WGS84 +ellps=WGS84" -t_srs EPSG:3857 -of GTiff -overwrite FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif ${year}$(printf "%.3d" $jday)_IDN_FireRisk.tif

	
		rm FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_temp.tif

	
	
		cd ${SCRIPTS_PATH}	
		##############################################
	
			if [ -f ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ]
			then
				${home_dir}/rescale_dslr_nas ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN.bin ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.bin ${npixels} 40

				#convert -depth 8 -size 555x444 gray:${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.bin ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
				#convert ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png $SCRIPTS_PATH/CLUT/gradient_paper.png -elut ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
				#convert ${DATA_PATH}/IDN/MASK/savannah_mask.png ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png -eompose Screen -eomposite -fuzz 20000 -fill gray92 -opaque white masked.png
				#convert ${DATA_PATH}/IDN/MASK/negate.png masked.png  -eompose Screen -eomposite  masked.png 
				#convert ${DATA_PATH}/IDN/MASK/IDN_land-elv_mask_negate.png masked.png  -eompose Multiply  -eomposite -fill white -opaque black  ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
				#convert ${DATA_PATH}/IDN/MASK/countries.png ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png -eompose Screen -eomposite ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
				#rm masked.png
				#convert ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png $SCRIPTS_PATH/CLUT/percent_moisture_vert_right.png -background white  -flip +append -flip  ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png

			else
				cp ${DATA_PATH}/IDN/MASK/missing_file.png  ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $jday)_${pass}_IDN_40.png
			fi
            
            		
            if [[ $jday == 1 ]]
            then
                gdal_translate -ot Byte -of EHdr "${outputdirectory}DSLR/2015/DSLR_2015365_IDN.bin" "${outputdirectory}DSLR/2016/DSLR_2015365_IDN_Last.bin"

            else
                gdal_translate -ot Byte -of EHdr "${outputdirectory}DSLR/${year}/DSLR_${year}$(printf "%.3d" $(( $jday-1 )))_IDN.bin" "${outputdirectory}DSLR/${year}/DSLR_${year}$(printf "%.3d" $(( $jday-1 )))_IDN_Last.bin"
            fi
            
            cd /firecast/DATA/IDN/MODOUT/DSLR/${year}/
            
            if [[ $jday == 1 ]]
            then
                ${home_dir}/days_since_last_rain_AMZEX DSLR_2015365_IDN_Last.bin /firecast/DATA/IDN/IMERG/$year/daysum/ppt_IMERG30m_early_${year}$(printf "%.3d" $jday)_sum_IDN.bin DSLR_${year}$(printf "%.3d" $jday)_IDN.bin 5 ${nrows} ${ncols}
            else
                ${home_dir}/days_since_last_rain_AMZEX DSLR_${year}$(printf "%.3d" $(( $jday-1 )))_IDN_Last.bin /firecast/DATA/IDN/IMERG/$year/daysum/ppt_IMERG30m_early_${year}$(printf "%.3d" $jday)_sum_IDN.bin DSLR_${year}$(printf "%.3d" $jday)_IDN.bin 5 ${nrows} ${ncols}
            fi
            cp /$DATA_PATH/IDN/gtifheader/UNSIGNED8.hdr ${outputdirectory}DSLR/${year}/DSLR_${year}$(printf "%.3d" $jday)_IDN.hdr
            gdal_translate -a_srs EPSG:4326 -ot Byte -of GTiff -stats -strict DSLR_${year}$(printf "%.3d" $jday)_IDN.bin DSLR_${year}$(printf "%.3d" $(( $jday )))_IDN_Temp.tif
            gdalwarp -s_srs EPSG:4326 -t_srs EPSG:3857 -ot Byte -of GTiff -overwrite DSLR_${year}$(printf "%.3d" $jday)_IDN_Temp.tif DSLR_${year}$(printf "%.3d" $jday)_IDN.tif
            rm DSLR_${year}$(printf "%.3d" $jday)_IDN_Temp.tif
            #rm "DSLR_${year}$(printf "%.3d" $(( $jday - 1 )))_IDN_Last.*"
            cd $HOME   

			### add to data base ###
			if [ "$enterdatabase" -eq 1 ]
			then
				#enter tiff into s3 database
				aws s3 cp "${outputdirectory}FR_INDEX/${year}/${year}$(printf "%.3d" $jday)_IDN_FireRisk.tif" "s3://suitability-mapper/fire-risk/IDN/"
				aws s3 cp "${outputdirectory}DSLR/${year}/DSLR_${year}$(printf "%.3d" $jday)_IDN.tif" "s3://suitability-mapper/fire-risk/IDN/DSLR/"
			    echo ""
			fi
	fi #end if moisture loop

	cd $HOME
    echo "PROCESS FINISHED"
exit $?

