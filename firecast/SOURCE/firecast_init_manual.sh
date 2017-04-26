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
#	2) MOD7Downloader.jar - searches the MOD07 geolocation file on ladsweb.nascom.nasa.gov for time codes associated 
#	with our Amazon window.  Once the time codes are found, it gerenates a list of MOD07 files for that day with 
#	the appropriate time codes to be downloaded.  The list of files names is written to "/firecast/DATA/<region>/MOD07L2/YYYY/MOD07_L2.AYYYYJJJ<region>.txt"
#
#	3) get_MOD07.sh reads filenames from textfile "/firecast/DATA/<region>/MOD07L2/YYYY/MOD07_L2.AYYYYJJJ<region>.txt" and pulls 
#	files from ftp site ladsweb.nascom.nasa.gov saving them to /firecast/DATA/<region>/MOD07L2/YYYY/
#	
#	4) AMZEX_MOD07_FIRECAST_manual_nas_gdal_win2_plevels_lm.sh - Generates batch files for HDFLook to execute to mosaic and subset the MOD07 HDF files. 
# 	Then spatially interpolates temperature and dew point files.  
#	Calculates Relatives Humidity. 
#	Generates litter moisture for 100 and 10000 hour times classes.
#	Uses GDAL to reproject Fire Risk Index GeoTiff to EPSG:3857 
#	Enters model output into s3 AWS bucket using AWS-CLI

####track mem use
#cat /proc/cpuinfo
#cat /proc/meminfo
#mpiexec -np X /u/scicon/tools/bin/gm.x -rss -c600 ./a.out

#### Shell Options ####
#enterdatabase flag 
#0=no records will be sent to the database.
#1=All Output will be sent to configured destination.
export set enterdatabase=1

#### AWS Credentials ####
export set AWS_ACCESS_KEY_ID="AKIAJHZDBGZJXSJTDHCA"
export set AWS_SECRET_ACCESS_KEY="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"
export set AWS_DEFAULT_REGION="us-east-1"
export set AWS_DEFAULT_PROFILE="default"
export set AWS_CONFIG_FILE="/firecast/.aws/config"

#### PATH Variables ####
export set HOME="/firecast"
export set PATH="$HOME/BIN:/usr/local/bin:/usr/local/sbin:$PATH"
export set DATA_PATH="$HOME/DATA"
export set SCRIPTS_PATH="$HOME/SCRIPTS"
export set GASCRP="$HOME/lib/gscript"

#### Date Variables ####
export set year=$( date -d yesterday +%Y )
export set yr=$( date -d yesterday +%y )
export set beginjday=$( date -d yesterday +%j )
export set endjday=$( date -d yesterday +%j )
export set prevjday=$( date -d "$year-01-01 +$(( $beginjday - 2 ))days" +%j)

    ### loops through Julian days ###
    ### In production environment only current day is processed ###
	for (( jday = $beginjday; jday <= $endjday; jday++ ))
	do
	    # prevyear is the year of the previous julian day.
	    # which is the same as $year unless beginjday is 001.
        export set prevyear=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%Y)
		export set jday=$jday
		
		#####  convert jday to YYYY-MM-DD  #####
        export set fulldate=$( date -d "$year-01-01 +$(( $jday - 1 ))days" +%Y-%m-%d )
        
        printf "\n***** AMZEX and IDN Fire Risk for $fulldate *****\n\n"
		
		#####  Download MOD07 and IMERG data for jday  #####
        cd $HOME/BIN
        java -jar MOD7Downloader.jar $DATA_PATH/AMZEX/MOD07L2/${year}/MOD07_L2.A${year}$(printf "%.3d" $jday)amzex.txt amazon ${fulldate}
        java -jar MOD7Downloader.jar $DATA_PATH/IDN/MOD07L2/${year}/MOD07_L2.A${year}$(printf "%.3d" $jday)idn.txt indonesia ${fulldate}
        $SCRIPTS_PATH/get_MOD07.sh $year $(printf "%.3d" $jday)
        $SCRIPTS_PATH/get_MOD07_IDN.sh $year $(printf "%.3d" $jday)
        rm $DATA_PATH/AMZEX/MOD07L2/${year}/MOD07_L2.A${year}$(printf "%.3d" $jday)amzex.txt
        rm $DATA_PATH/IDN/MOD07L2/${year}/MOD07_L2.A${year}$(printf "%.3d" $jday)idn.txt
        cd $HOME
        $SCRIPTS_PATH/get_IMERG_HDF5.sh
		
        #####  Subset IMERG HDF5 and calculate durations/sums  #####
        $SCRIPTS_PATH/AMZEX_IMERG_manual_GrADs.sh
		$SCRIPTS_PATH/IDN_IMERG_manual_GrADs.sh
		
		cd $HOME
        #####  Run fire risk models  #####
        $SCRIPTS_PATH/AMZEX_MOD07_IMERG_FIRECAST_manual_nas_gdal_win2_plevels_lm.sh 
		$SCRIPTS_PATH/IDN_MOD07_IMERG_FIRECAST_manual_nas_gdal_win2_plevels_lm.sh
		
		export set prevjday=$jday
		
	done
#cat /proc/cpuinfo
#cat /proc/meminfo
printf "\n\n`env`\n\n"
#aws ec2 stop-instances --instance-ids i-f1c40824
shutdown now
exit 0
