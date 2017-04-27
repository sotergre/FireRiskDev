#! /bin/bash
#firecast_init_manual.sh is the mainframe to the fire risk model in manual mode for rerunning days
#This script downloads MODIS MOD07 data and runs the fire risk model that day.  
#So far this script does not include TRMM rainfall processing.  TRMM is processed seperately with TRMM3B42RT_cron_GMTminus5.sh
#
#This scipt requires to two other executables 1) HDFLook 2) ImageMagick
#And calls to 3 other scrips. 1)AMZEX_TRMM_manual_GrADs.sh 2)get_MOD07.sh 3) AMZEX_MOD07_FIRECAST_manual_nas_gdal_win2_plevels_lm.sh
#	1)AMZEX_TRMM_manual_GrADs.sh - Subsets and clips TRMM binary to AMZEX window using GrADS.
#   Then uses 'gdalwarp' to resample the grid by "nearest neighbor" to match MOD07 data grid.
#   Then uses 'dursum_3B42RT_AMZEX_32_win2' executable to create duration and summation binaries.
#
#	2) MOD7Downloader.jar - searches the MOD03 geolocation file on ladsweb.nascom.nasa.gov for time codes associated 
#	with our Amazon window.  Once the time codes are found, it gerenates a list of MOD07 files for that day with 
#	the appropriate time coes to be downloaded.  The list of files names is written to "/firecast/DATA/AMZEX/MOD07L2/YYYY/MOD07_L2.AYYYYJJJ.txt"
#
#	3) get_MOD07.sh reads filenames from textfile "/firecast/DATA/AMZEX/MOD07L2/YYYY/MOD07_L2.AYYYYJJJ.txt" and pulls 
#	files from ftp site ladsweb.nascom.nasa.gov saving them to /firecast/DATA/AMZEX/MOD07L2/YYYY/
#	
#	4) AMZEX_MOD07_FIRECAST_manual_nas_gdal_win2_plevels_lm.sh - Generates batch files for HDFLook to execute to mosaic and subset the MOD07 HDF files. 
# 	Then spatially interpolates temperature and dew point files.  
#	Calculated Relatives Humidity. 
#	Generates litter moisture for 10, 100, and 10000 hour times calsses.
#	Uses imagemagick to creates PNG's from binaries 
#	Enters model outputs (both binary and PNG) into database

####track mem use
cat /proc/cpuinfo
cat /proc/meminfo
#mpiexec -np X /u/scicon/tools/bin/gm.x -rss -c600 ./a.out

#### Shell Options #####
#enterdatabase flag 
#0=no records will be entered into database
#1=to record only BIN to image data base to avoid duplicating PNG entry to image database /usr/local/bin/saveimagedb.pl
#2=to record PNG and BIN outputs to image database /usr/local/bin/saveimagedb.pl
export set HOME="/firecast"
export set PATH="/opt/local/bin:/opt/local/sbin:$HOME/BIN:$PATH"
export set DATA_PATH="$HOME/DATA"
export set SCRIPTS_PATH="$HOME/SCRIPTS"
export set enterdatabase=0
export set year=2015
export set yr=$(expr $year - 2000)
export set beginjday=244
export set endjday=244


export set prevjday=243
julian () { date -d "$1-01-01 +$(( $2 - 1  ))days" +%Y-%m-%d; }


########################
#last year is the previous year from $year
lastyear=$(expr $year - 1)

### loops through Julian days ###
	for (( jday = $beginjday; jday <= $endjday; jday++ ))
	do
	    #prevyear is the year of the previous julian day which is the same year unless beginjday is 1.
        export set prevyear=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%Y)
		export jday=$jday
		
		#####  convert jday to YYYY-MM-DD  #####
        #tempjulianday=$(. $SCRIPTS_PATH/julianconvert.ksh -j $lastyear 12 31)
        #julianday=$(expr $tempjulianday + $jday)
        fulldate=`julian $year $jday`
        export set fulldate=$fulldate
        
        
        printf "\n***** FireRisk : $fulldate *****\n"
		
		#####  Download MOD07 and TRMM data for jday  #####
        cd /firecast/BIN
        java -jar MOD7Downloader.jar /firecast/DATA/AMZEX/MOD07L2/${year}/MOD07_L2.A${year}$(printf "%.3d" $jday)amzex.txt amazon ${fulldate}
        $SCRIPTS_PATH/get_MOD07.sh $year $(printf "%.3d" $jday)
        rm /firecast/DATA/AMZEX/MOD07L2/${year}/MOD07_L2.A${year}$(printf "%.3d" $jday)amzex.txt
        cd /firecast
        $SCRIPTS_PATH/get_IMERG_HDF5.sh
		#wait for ftp to finish before executing next script
        #wait
        # Run fire model after aquisition of data
        
        #####  Subset IMERG GeoTIFF and calculate durations/sums  #####
        $SCRIPTS_PATH/AMZEX_IMERG_manual_GrADs.sh
		
		cd $HOME
        
        $SCRIPTS_PATH/AMZEX_MOD07_IMERG_FIRECAST_manual_nas_gdal_win2_plevels_lm.sh $year $jday $prevyear $prevjday $enterdatabase
		
		
		
		export set prevjday=$jday
		
	done
printf "\n\n`env`\n\n"
exit
