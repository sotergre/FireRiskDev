
#! /bin/zsh
date
year=2012
beginjday=179
endjday=190
prevyear=2012
prevday=178

#################################################################
#	This shell generates a batch script for HDFLook		#
#	last updated FEBUARY 27th 2008
#	 							#
#				#
#								#
#	batch files called smoff{year}{jday}HDFlook.batch	#
#								#
#	output projection paramters are in 			#
#	"LST_${prefix}07_${year}${jday}_${pass}_AMZ.hdr"	#
#	"DPT_${prefix}07_${year}${jday}_${pass}_AMZ.hdr"	#
#	"LST_${prefix}07_${year}${jday}_${pass}_AMZ.hdr"	#
#################################################################


#################################################################
#		User Defined Parameters				#
#################################################################

#prefix is either MOD or MYD
prefix=MOD 

#pass is either Day or Night
pass=Day

######################
#   FLAGS		#

#if 1 then HDF files are processed from HDF and reprojected
#change to 0 if the binaries already exist
process=0

#interpolate flag - 1 interpolate on | 0 interpolate off
interpolate=0

#RH flag - 1 calc RH on | 0 calc RH off
calcRH=0

#calculate litter moisture flag - 1 on | 0 off
moisture=1

#add litter moisture data and images to database - 1 on | 0 off
add_database=0

#####################

###set geographic parameters in DD###
UL_X=-75.0
UL_Y=0.0
LR_X=-50.0
LR_Y=-20.0

####pixel size 1000m=0.008333 and 5000m=0.041665
pixel_size_DD=0.041665
pixels_size_m=5000

####nrows = ABS( (UL_X-LR_X)/ pixel_size_DD )
nrows=444
####ncolss = ABS( (UL_X-LR_X)/ pixel_size_DD )
ncols=555
#nrows*ncols
npixels=246420

#####################################
########set directories#############

####executable directory###
home_dir="/FIRERISK/BIN/"

HDFbinDIR="/bin/HDFLOOK_linux/"

####mask name and location###
mask="/FIRERISK/DATA/AMZ/MASK/demmask_330m_win1_filt_noshore.bin"

###MOD07 input directory####
MOD07_indir="/FIRERISK/DATA/AMZ/${prefix}07L2/"

##### Root Out directory   ####
outputdirectory="/FIRERISK/DATA/AMZ/${prefix}OUT/"

##initialize previousLM
prevLMfile10=${outputdirectory}LM/${prevyear}/HR0010/LM0010h_${prevyear}$(printf "%.3d" $prevday)_${pass}_AMZ.bin
prevLMfile100=${outputdirectory}LM/${prevyear}/HR0100/LM0100h_${prevyear}$(printf "%.3d" $prevday)_${pass}_AMZ.bin
prevLMfile1000=${outputdirectory}LM/${prevyear}/HR1000/LM1000h_${prevyear}$(printf "%.3d" $prevday)_${pass}_AMZ.bin


###############   TRMM         #################
####	rainfall input directory 		#######
trmminputdir="/FIRERISK/DATA/AMZ/3B42RT/"



#########		end user defined paramters	###########
###############################################################						
	
### loops through Julian days ###
for (( day = $beginjday; day <= $endjday; day++ ))
	do
		
	#####################generate batch files for HDFLook and execute ###################
	##########	extracts AT and DPT from Pressure Profile 20	#####################
	##########	Projects AT and DPT to Sinusoidal			#####################

	if [ "$process" -eq 1 ]
	then
	profile=20
	########		generate batch file for Dew Point Profile		###############
	echo "verbose" > ${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch
	echo "clear_data" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch
	echo "" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch
	echo "set_input_directory    ${MOD07_indir}${year}/" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch
	echo "set_output_directory    ${outputdirectory}TP_PROFILE/${year}/" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch
	echo "" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch
	echo 'set_projection_to_geometry   ProjectionTo="Sinusoidal" WidthTo='$ncols' HeightTo='$nrows'  PixelSizeXTo='$pixels_size_m' PixelSizeYTo='$pixels_size_m' LatitudeMinTo='$LR_Y' LatitudeMaxTo='$UL_Y' LongitudeMinTo='$UL_X' LongitudeMaxTo='$LR_X' '  >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch
	echo "set_input_hdf_file         ${MOD07_indir}${year}/${prefix}07_L2.A${year}$(printf "%.3d" $day)*" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch 
	echo "select_SDS                 SDSName= \"Retrieved_Moisture_Profile\" Index=${profile} " >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch
	echo "set_misc_options Only${pass}=Yes" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch
	echo "create_MODIS_SDS_MOSAIC    FileName=\"DPT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin\" ClearFile=Yes scaling=\"y=a(x-b)\" Index=${profile}" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch
	
	${HDFbinDIR}HDFLook ${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch
	mv ${outputdirectory}TP_PROFILE/${year}/DPT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin_20 ${outputdirectory}TP_PROFILE/${year}/DPT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin
	rm ${outputdirectory}TP_PROFILE/${year}/${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch_STATUS
	rm ${prefix}07${year}$(printf "%.3d" $day)MOD07L2DPT.batch
	
	########		generate batch file for Atmospheric Temperature profile 	###############
	echo "verbose" > ${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch
	echo "clear_data" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch
	echo "" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch
	echo "set_input_directory    ${MOD07_indir}${year}/" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch
	echo "set_output_directory    ${outputdirectory}TP_PROFILE/${year}/" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch
	echo "" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch
	echo 'set_projection_to_geometry   ProjectionTo="Sinusoidal" WidthTo='$ncols' HeightTo='$nrows' PixelSizeXTo='$pixels_size_m' PixelSizeYTo='$pixels_size_m' LatitudeMinTo='$LR_Y' LatitudeMaxTo='$UL_Y' LongitudeMinTo='$UL_X' LongitudeMaxTo='$LR_X' ' >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch
	echo "set_input_hdf_file         ${MOD07_indir}${year}/${prefix}07_L2.A${year}$(printf "%.3d" $day)*" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch
	echo "select_SDS                 SDSName= \"Retrieved_Temperature_Profile\" Index=${profile}" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch
	echo "set_misc_options Only${pass}=Yes" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch
	echo "create_MODIS_SDS_MOSAIC    FileName=\"AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin\" ClearFile=Yes scaling=\"y=a(x-b)\" Index=${profile}" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch
	
	###  execute AT script  ###
	${HDFbinDIR}HDFLook ${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch
	mv ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin_20 ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin
	rm ${outputdirectory}TP_PROFILE/${year}/${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch_STATUS
	rm ${prefix}07${year}$(printf "%.3d" $day)MOD07L2AT.batch



	########	generate batch file for MODIS 07L2 Surface Temperature ###############

	echo "verbose" > ${prefix}07${year}$(printf "%.3d" $day)MOD07L2SFT.batch
	echo "clear_data" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2SFT.batch
	echo "" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2SFT.batch
	echo "set_input_directory    ${MOD07_indir}${year}/" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2SFT.batch
	echo "set_output_directory   ${outputdirectory}SFT/${year}/" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2SFT.batch
	echo "" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2SFT.batch	
	echo 'set_projection_to_geometry   ProjectionTo="Sinusoidal" WidthTo='$ncols' HeightTo='$nrows'  PixelSizeXTo='$pixels_size_m' PixelSizeYTo='$pixels_size_m' LatitudeMinTo='$LR_Y' LatitudeMaxTo='$UL_Y' LongitudeMinTo='$UL_X' LongitudeMaxTo='$LR_X' '  >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2SFT.batch	
	echo "set_input_hdf_file         ${MOD07_indir}${year}/${prefix}07_L2.A${year}$(printf "%.3d" $day)*" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2SFT.batch
	echo "select_SDS                 SDSName= \"Surface_Temperature\" " >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2SFT.batch
	echo "create_MODIS_SDS_MOSAIC    FileName=\"SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin\" ClearFile=Yes scaling=\"y=a(x-b)\" Index=1" >> ${prefix}07${year}$(printf "%.3d" $day)MOD07L2SFT.batch
	
	###  execute SFT script  ###
	${HDFbinDIR}HDFLook  ${prefix}07${year}$(printf "%.3d" $day)MOD07L2SFT.batch
	rm ${outputdirectory}SFT/${year}/${prefix}07${year}$(printf "%.3d" $day)MOD07L2SFT.batch_STATUS
	rm ${prefix}07${year}$(printf "%.3d" $day)MOD07L2SFT.batch
		
	echo "****************************** "
	echo "Finished extracting HDF data"
	echo "****************************** "

	fi



	if [ "$interpolate" -eq 1 ]
	then
	######	interpolate	#####
		if [ -f ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ]
		then

		${home_dir}interpolate_MOD07_TP ${mask} ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}TP_PROFILE/${year}/DPT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ${outputdirectory}TP_PROFILE/${year}/DPT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1.bin ${ncols} ${nrows} -1 1 3 1000
		${home_dir}dw_interp_MOD07_TP ${mask} ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}TP_PROFILE/${year}/DPT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}TP_PROFILE/${year}/DPT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis2.bin ${ncols} ${nrows} 0 

		#convert MOD07 AT and DPT
		${home_dir}convertKx10toC ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.bin ${npixels} 0 40
		convert -depth 8 -size 555x444 gray:${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.bin ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png
		convert ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png /FIRERISK/SCRIPTS/CLUT/gradient_lst.png -clut ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png
		convert /FIRERISK/DATA/AMZ/MASK/elv_mask_negate.png ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png -compose Screen  -composite  masked.png
		convert /FIRERISK/DATA/AMZ/MASK/amz_land-elv_mask_negate.png masked.png -compose Multiply  -composite  -fill white -opaque black  ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png
		convert /FIRERISK/DATA/AMZ/MASK/countries.png ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png -compose Screen -composite ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png
		rm masked.png

		#convert MOD07 TP distance file distance 100 pixels = 500km (pixels*5km)
		${home_dir}convert_sint2uchar ${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1.bin ${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.bin ${npixels} 0 100
		convert -depth 8 -size 555x444 gray:${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.bin ${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png
		convert ${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png /FIRERISK/SCRIPTS/CLUT/gradient_lst.png -clut ${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png
		convert /FIRERISK/DATA/AMZ/MASK/elv_mask_negate.png ${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png -compose Screen  -composite  masked.png
		convert /FIRERISK/DATA/AMZ/MASK/amz_land-elv_mask_negate.png masked.png -compose Multiply  -composite  -fill white -opaque black  ${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png
		convert /FIRERISK/DATA/AMZ/MASK/countries.png ${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png -compose Screen -composite ${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png
		rm masked.png
		convert ${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png /FIRERISK/SCRIPTS/CLUT/interp_disance_vert_right.png -background white  -flip +append -flip  ${outputdirectory}TP_PROFILE/${year}/TP_PROFILE_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png

		fi
	
		if [ -f ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ]
		then
		${home_dir}interpolate_MOD07_SFT ${mask} ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1.bin ${ncols} ${nrows} 0 1 3 1000
		${home_dir}dw_interp_MOD07_SFT ${mask} ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis2.bin ${ncols} ${nrows} 0 
		#convert MOD07 SFT
		${home_dir}convertKx10toC ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.bin ${npixels} 0 40
		convert -depth 8 -size 555x444 gray:${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.bin ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png
		convert ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png /FIRERISK/SCRIPTS/CLUT/gradient_lst.png -clut ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png
		convert /FIRERISK/DATA/AMZ/MASK/elv_mask_negate.png ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png -compose Screen  -composite  masked.png
		convert /FIRERISK/DATA/AMZ/MASK/amz_land-elv_mask_negate.png masked.png -compose Multiply  -composite  -fill white -opaque black  ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png
		convert /FIRERISK/DATA/AMZ/MASK/countries.png ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png -compose Screen -composite ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png
		rm masked.png

		#convert MOD07 SFT distance file ditance 1000 pixels = 500km (pixels/10*5km)
		${home_dir}convert_sint2uchar ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1.bin ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.bin ${npixels} 0 1000
		convert -depth 8 -size 555x444 gray:${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.bin ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png
		convert ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png /FIRERISK/SCRIPTS/CLUT/gradient_lst.png -clut ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png
		convert /FIRERISK/DATA/AMZ/MASK/elv_mask_negate.png ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png -compose Screen  -composite  masked.png
		convert /FIRERISK/DATA/AMZ/MASK/amz_land-elv_mask_negate.png masked.png -compose Multiply  -composite  -fill white -opaque black  ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png
		convert /FIRERISK/DATA/AMZ/MASK/countries.png ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png -compose Screen -composite ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png
		rm masked.png
		convert ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png /FIRERISK/SCRIPTS/CLUT/interp_disance_vert_right.png -background white  -flip +append -flip  ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png

		else
		cp /FIRERISK/DATA/AMZ/MASK/missing_file.png ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_40.png
		cp /FIRERISK/DATA/AMZ/MASK/missing_file.png ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_dis1_500km.png
		fi

	echo "****************************** "
	echo "Finished interpolation"
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

	${home_dir}rhumid_at_dpt  ${outputdirectory}TP_PROFILE/${year}/DPT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}TP_PROFILE/${year}/AT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${npixels}

		#convert MOD07 RH
		if [ -f ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ]
		then
		${home_dir}convert1000pto100p ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_20to100.bin ${npixels} 20 100
		convert -depth 8 -size 555x444 gray:${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_20to100.bin ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_20to100.png
		convert ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_20to100.png /FIRERISK/SCRIPTS/CLUT/gradient_all.png -clut ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_20to100.png
		convert /FIRERISK/DATA/AMZ/MASK/elv_mask_negate.png ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_20to100.png -compose Screen  -composite  masked.png
		convert /FIRERISK/DATA/AMZ/MASK/amz_land-elv_mask_negate.png masked.png -compose Multiply  -composite  -fill white -opaque black  ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_20to100.png
		convert /FIRERISK/DATA/AMZ/MASK/countries.png ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_20to100.png -compose Screen -composite ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_20to100.png
		rm masked.png
		else
		cp /FIRERISK/DATA/AMZ/MASK/missing_file.png  ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1_20to100.png
		fi

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
	echo "Calculating 10 HR"
	echo ""
	${home_dir}fmodel_10hr ${trmminputdir}${prevyear}/15_hrdur/ppt_3b42rt_${prevyear}$(printf "%.3d" $prevday)_dur_AMZ_p1.bin ${trmminputdir}${prevyear}/09_hrdur/ppt_3b42rt_${prevyear}$(printf "%.3d" $prevday)_dur_AMZ_p2.bin  ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${prevLMfile10}  ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ${npixels} 
        

		if [ -f ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ]
		then
		echo "Convert 10 HR moisture to PNG"
		#convert LM0010 to png
		${home_dir}rescale_dslr ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.bin ${npixels} 40
		convert -depth 8 -size 555x444 gray:${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.bin ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		convert ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png /FIRERISK/SCRIPTS/CLUT/gradient_all.png -clut ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		convert /FIRERISK/DATA/AMZ/MASK/savannah_mask.png ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png -compose Screen -composite -fuzz 20000 -fill gray92 -opaque white masked.png
		convert /FIRERISK/DATA/AMZ/MASK/negate.png masked.png  -compose Screen -composite  masked.png 
		convert /FIRERISK/DATA/AMZ/MASK/amz_land-elv_mask_negate.png masked.png  -compose Multiply  -composite -fill white -opaque black  ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		convert /FIRERISK/DATA/AMZ/MASK/countries.png ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png -compose Screen -composite ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		rm masked.png
		prevLMfile10=${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin 

		else
		cp /FIRERISK/DATA/AMZ/MASK/missing_file.png  ${outputdirectory}LM/${year}/HR0010/LM0010h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		fi

	echo "prevLM10 is file is $prevLMfile10"

	echo ""
	echo "Calculating 100 HR"
	echo ""
	${home_dir}fmodel_100hr_1000hr ${trmminputdir}${prevyear}/24_hrdur/ppt_3b42rt_${prevyear}$(printf "%.3d" $prevday)_dur_AMZ.bin ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${prevLMfile100} ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ${npixels} 100

		if [ -f ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ]
		then
		echo "Convert 100 HR moisture to PNG"
		#convert LM0100 to png
		${home_dir}rescale_dslr ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.bin ${npixels} 40
		convert -depth 8 -size 555x444 gray:${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.bin ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		convert ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png /FIRERISK/SCRIPTS/CLUT/gradient_all.png -clut ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		convert /FIRERISK/DATA/AMZ/MASK/savannah_mask.png ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png -compose Screen -composite -fuzz 20000 -fill gray92 -opaque white masked.png
		convert /FIRERISK/DATA/AMZ/MASK/negate.png masked.png  -compose Screen -composite  masked.png 
		convert /FIRERISK/DATA/AMZ/MASK/amz_land-elv_mask_negate.png masked.png  -compose Multiply  -composite -fill white -opaque black  ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		convert /FIRERISK/DATA/AMZ/MASK/countries.png ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png -compose Screen -composite ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		rm masked.png

		prevLMfile100=${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin 

		else
		cp /FIRERISK/DATA/AMZ/MASK/missing_file.png  ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		fi
	echo "prevLMfile100 is $prevLMfile100 "
	
	echo ""
	echo "Calculating 1000 HR"
	echo ""
	${home_dir}fmodel_100hr_1000hr ${trmminputdir}${prevyear}/24_hrdur/ppt_3b42rt_${prevyear}$(printf "%.3d" $prevday)_dur_AMZ.bin ${outputdirectory}RH/${year}/RH_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${outputdirectory}SFT/${year}/SFT_${prefix}07_${year}$(printf "%.3d" $day)_${pass}_AMZ_int1.bin ${prevLMfile1000} ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ${npixels} 1000
		if [ -f ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ]
		then
		echo "Convert 10 HR moisture to PNG"
		#convert LM1000 to png
		${home_dir}rescale_dslr ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.bin ${npixels} 40
		convert -depth 8 -size 555x444 gray:${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.bin ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		convert ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png /FIRERISK/SCRIPTS/CLUT/gradient_all.png -clut ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		convert /FIRERISK/DATA/AMZ/MASK/savannah_mask.png ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png -compose Screen -composite -fuzz 20000 -fill gray92 -opaque white masked.png
		convert /FIRERISK/DATA/AMZ/MASK/negate.png masked.png  -compose Screen -composite  masked.png 
		convert /FIRERISK/DATA/AMZ/MASK/amz_land-elv_mask_negate.png masked.png  -compose Multiply  -composite -fill white -opaque black  ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		convert /FIRERISK/DATA/AMZ/MASK/countries.png ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png -compose Screen -composite ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		rm masked.png

		prevLMfile1000=${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin 

		else
		cp /FIRERISK/DATA/AMZ/MASK/missing_file.png  ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		fi
	echo "prevLMfile1000 is $prevLMfile1000"

	echo ""
	echo "Calculating Fire Risk Index"
	echo ""
	##########generate fire risk#########
	${home_dir}firerisk_index ${outputdirectory}LM/${year}/HR0100/LM0100h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ${outputdirectory}LM/${year}/HR1000/LM1000h_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ${npixels}
	
		if [ -f ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ]
		then
		${home_dir}rescale_dslr ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.bin ${npixels} 40
		convert -depth 8 -size 555x444 gray:${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.bin ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		convert ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png /FIRERISK/SCRIPTS/CLUT/gradient_paper.png -clut ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		convert /FIRERISK/DATA/AMZ/MASK/savannah_mask.png ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png -compose Screen -composite -fuzz 20000 -fill gray92 -opaque white masked.png
		convert /FIRERISK/DATA/AMZ/MASK/negate.png masked.png  -compose Screen -composite  masked.png 
		convert /FIRERISK/DATA/AMZ/MASK/amz_land-elv_mask_negate.png masked.png  -compose Multiply  -composite -fill white -opaque black  ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		convert /FIRERISK/DATA/AMZ/MASK/countries.png ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png -compose Screen -composite ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		rm masked.png
		convert ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png /FIRERISK/SCRIPTS/CLUT/percent_moisture_vert_right.png -background white  -flip +append -flip  ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png

		else
		cp /FIRERISK/DATA/AMZ/MASK/missing_file.png  ${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png
		fi
		
		if [ "$add_database" -eq 1 ]
		then
		### add to data base ###
		/usr/local/bin/saveimagedb.pl -p "${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ.bin" -t 2 -f 1 -r 0 -j "${year}$(printf "%.3d" $day)"
		/usr/local/bin/saveimagedb.pl -p "${outputdirectory}FR_INDEX/${year}/FIRERISK_${year}$(printf "%.3d" $day)_${pass}_AMZ_40.png" -t 2 -f 0 -r 0 -j "${year}$(printf "%.3d" $day)"
		fi
	fi

	prevyear=${year}
	prevday=${day}		

		


done
	date
        echo "PROCESS FINISHED"