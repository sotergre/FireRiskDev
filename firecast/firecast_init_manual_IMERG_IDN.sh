#! /bin/bash
#firecast_init_manual.sh is the mainframe to the fire risk model in manual mode for rerunning days
#This script downloads MODIS MOD07 data and runs the fire risk model that day.  
#So far this script does not include TRMM rainfall processing.  TRMM is processed seperately with TRMM3B42RT_cron_GMTminus5.sh
#
#This scipt requires to two other executables 1) HDFLook 2) ImageMagick
#And calls to 3 other scrips. 1)IDN_TRMM_manual_GrADs.sh 2)get_MOD07.sh 3) IDN_MOD07_FIRECAST_manual_nas_gdal_win2_plevels_lm.sh
#	1)IDN_TRMM_manual_GrADs.sh - Subsets and clips TRMM binary to IDN window using GrADS.
#   Then uses 'gdalwarp' to resample the grid by "nearest neighbor" to match MOD07 data grid.
#   Then uses 'dursum_3B42RT_IDN_32_win2' executable to create duration and summation binaries.
#
#	2) MOD7Downloader.jar - searches the MOD03 geolocation file on ladsweb.nascom.nasa.gov for time codes associated 
#	with our Amazon window.  Once the time codes are found, it gerenates a list of MOD07 files for that day with 
#	the appropriate time coes to be downloaded.  The list of files names is written to "/firecast/DATA/IDN/MOD07L2/YYYY/MOD07_L2.AYYYYJJJ.txt"
#
#	3) get_MOD07.sh reads filenames from textfile "/firecast/DATA/IDN/MOD07L2/YYYY/MOD07_L2.AYYYYJJJ.txt" and pulls 
#	files from ftp site ladsweb.nascom.nasa.gov saving them to /firecast/DATA/IDN/MOD07L2/YYYY/
#	
#	4) IDN_MOD07_FIRECAST_manual_nas_gdal_win2_plevels_lm.sh - Generates batch files for HDFLook to execute to mosaic and subset the MOD07 HDF files. 
# 	Then spatially interpolates temperature and dew point files.  
#	Calculated Relatives Humidity. 
#	Generates litter moisture for 10, 100, and 10000 hour times calsses.
#	Uses imagemagick to creates PNG's from binaries 
#	Enters model outputs (both binary and PNG) into database

####track mem use
#cat /proc/cpuinfo
#cat /proc/meminfo
#mpiexec -np X /u/scicon/tools/bin/gm.x -rss -c600 ./a.out

#### Shell Options #####
#enterdatabase flag 
#0=no records will be sent to the database.
#1=All Output will be sent to configured destination.
export set enterdatabase=1

export set AWS_ACCESS_KEY_ID="AKIAJHZDBGZJXSJTDHCA"
export set AWS_SECRET_ACCESS_KEY="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"
export set AWS_DEFAULT_REGION="us-east-1"
export set AWS_DEFAULT_PROFILE="default"
export set AWS_CONFIG_FILE="/firecast/.aws/config"

export set HOME="/firecast"
export set PATH="$HOME/BIN:/usr/local/bin:/usr/local/sbin:$PATH"
export set DATA_PATH="$HOME/DATA"
export set SCRIPTS_PATH="$HOME/SCRIPTS"
export set GASCRP="$HOME/lib/gscript"


export set year=2015
#export set year=$( date -d yesterday +%Y )
export set yr=$(expr $year - 2000)
#export set yr=$( date -d yesterday +%y )
export set beginjday=91
#export set beginjday=$( date -d yesterday +%j )
#export set endjday=277
export set endjday=$( date -d yesterday +%j )


#export set prevjday=255




########################
#last year is the previous year from $year
lastyear=$(expr $year - 1)

### loops through Julian days ###
	for (( jday = $beginjday; jday <= $endjday; jday++ ))
	do
	    # prevyear is the year of the previous julian day.
	    # which is the same as $year unless beginjday is 001.
        export set prevyear=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%Y)
		export set jday=$jday
		export set prevjday=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%j)
		
		#####  convert jday to YYYY-MM-DD  #####
        export set fulldate=$( date -d "$year-01-01 +$(( $jday - 1 ))days" +%Y-%m-%d )
        
        printf "\n***** IDN Fire Risk for $fulldate *****\n\n"
		
		#####  Download MOD07 and IMERG data for jday  #####
        cd $HOME/BIN
        java -jar MOD7Downloader.jar ${DATA_PATH}/IDN/MOD07L2/${year}/MOD07_L2.A${year}$(printf "%.3d" $jday)idn.txt indonesia ${fulldate}
        $SCRIPTS_PATH/get_MOD07_IDN.sh $year $(printf "%.3d" $jday)
        rm ${DATA_PATH}/IDN/MOD07L2/${year}/MOD07_L2.A${year}$(printf "%.3d" $jday)idn.txt
        cd $HOME
        $SCRIPTS_PATH/get_IMERG_HDF5.sh
		
        #####  After downloading is complete subset IMERG HDF5 and calculate durations/sums  #####        
        $SCRIPTS_PATH/IDN_IMERG_manual_GrADs.sh
		
		cd $HOME
        # Run fire model after subsetting of data
        $SCRIPTS_PATH/IDN_MOD07_IMERG_FIRECAST_manual_nas_gdal_win2_plevels_lm.sh
		
		#export set prevjday=$jday
		
	done
cat /proc/cpuinfo
cat /proc/meminfo

printf "\n\n`env`\n\n"
exit