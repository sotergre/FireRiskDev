#!/bin/bash
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


export set arc=`arch`
####track mem use
if [ "$arc" = "i386" ]
then
    top -l 2 -o cpu -n 0
    
else
    cat /proc/cpuinfo
    cat /proc/meminfo
fi

#### Shell Options ####
#enterdatabase flag 
#0=Model will run and produce outputs but will not push them to the s3
#1=Two files will be sent to the s3 at the end of each day processed. FR_Index and DSLR
export set enterdatabase=1

#### AWS Credentials ####
#export set AWS_ACCESS_KEY_ID="***INSERT KEY BEFORE RUNNING***"
#export set AWS_SECRET_ACCESS_KEY="***INSERT KEY BEFORE RUNNING***"
export set AWS_DEFAULT_REGION="us-east-1"
export set AWS_DEFAULT_PROFILE="default"
export set AWS_CONFIG_FILE="/firecast/.aws/config"

#### PATH Variables ####
### HOME is the path to this file and can be configured                                   ###
export set HOME="/firecast"

### !!!Caution!!! Edit only if you have manually altered the directory structure          ###

if [ "$arc" = "i386" ]
then
    export set PATH="$HOME/BIN:/usr/local/bin:/usr/local/sbin:/opt/local/bin:/opt/local/sbin:$PATH"
else
    export set PATH="$HOME/BIN:/usr/local/bin:/usr/local/sbin:$PATH"
fi

export set DATA_PATH="$HOME/DATA"
export set SCRIPTS_PATH="$HOME/SCRIPTS"
export set GASCRP="$HOME/lib/gscript"
### !!!End Caution!!!                                                                     ###

#### Date Variables for gnu date (gdate) ####
if [ "$arc" = "i386" ]
then
    export set year=$( gdate -d yesterday +%Y )
    export set yr=$( gdate -d yesterday +%y )
    export set endjday=$( gdate -d yesterday +%-j )
else
#### Date Variables on gnu coreutils date ####
    export set year=$( date -d yesterday +%Y )
    export set yr=$( date -d yesterday +%y )
    export set endjday=$( date -d yesterday +%-j )
fi

### Starting with the nth day prior to today, check to make sure there are output files   ###
### When a file is not found, set that day as the first day to process                    ###
# Set to 32 to make sure all 32 previous files are there for temporal interpolation
export set n=32
if [ "$arc" = "i386" ]
then
    for (( i=$((n)); i>0; i-- ))
    do
    
        jdayOutCheck=$(gdate -d "yesterday +$(( 0 - i ))days" +%-j)

        if [ ! -f "${DATA_PATH}/HEMI/MODOUT/FR_INDEX/${year}/${year}$(printf "%.3d" $jdayOutCheck)_HEMI_FireRisk.tif" ]
        then
            echo "i=$i"
            echo "jdayOutCheck:$jdayOutCheck"
            i=-9999;
            echo "File Not Found: ${year}$(printf "%.3d" $jdayOutCheck)_HEMI_FireRisk.tif"
        
        else
            echo "Found ${year}$(printf "%.3d" $jdayOutCheck)_HEMI_FireRisk.tif";
        fi
    done
else

    for (( i=$((n)); i>0; i-- ))
    do
    
        jdayOutCheck=$(date -d "yesterday +$(( 0 - i ))days" +%-j)

        if [ ! -f "${DATA_PATH}/HEMI/MODOUT/FR_INDEX/${year}/${year}$(printf "%.3d" $jdayOutCheck)_HEMI_FireRisk.tif" ]
        then
            echo "i=$i"
            echo "jdayOutCheck:$jdayOutCheck"
            i=-9999;
            echo "File Not Found: ${year}$(printf "%.3d" $jdayOutCheck)_HEMI_FireRisk.tif"
        
        else
            echo "Found ${year}$(printf "%.3d" $jdayOutCheck)_HEMI_FireRisk.tif";
        fi
    done
fi


### If all files are present, jdayOutCheck's last assignment would have been for the day\ ###
### before yesterday thus needing to be advanced to yesterday before assinging beginjday  ###
if [ ! $i -eq -9999 ]
then
    export set beginjday=$jdayOutCheck
else
    export set beginjday=$(( $jdayOutCheck + 1 ))
fi

### Manual Override ###
# export set beginjday=110
env
#### Main Loop ####
### Loops through previous days starting at day with missing data but no more than $n days ###
### Most recent day processed is yesterday(UTC) e.g.:                                     ###
### If server time(UTC) is Oct 10 23:59:59, then model will process through Oct 9         ###
if [ "$arc" = "i386" ]
then
    for (( jday = $beginjday; jday <= $endjday; jday++ ))
    do
        ### prevyear is the year of the previous julian day.                                  ###
        ### which is the same as $year unless beginjday is 001.                               ###
        export set prevyear=$( gdate -d "$year-01-01 +$(( $jday - 2 ))days" +%Y)
    
        export set jday=$jday
        export set prevjday=$( gdate -d "$year-01-01 +$(( $jday - 2 ))days" +%-j)
        #####  convert jday to YYYY-MM-DD  #####
        export set fulldate=$( gdate -d "$year-01-01 +$(( $jday - 1 ))days" +%Y-%m-%d )
    
        printf "\n***** HEMI Fire Risk for $fulldate *****\n\n"
    
        #####  Download MOD07 and IMERG data for jday  #####
        cd $HOME/BIN
        java -jar MOD7DownloaderHEMI.jar $DATA_PATH/HEMI/MOD07L2/${year}/MOD07_L2.A${year}$(printf "%.3d" $jday)hemi.txt ${fulldate}
        
        $SCRIPTS_PATH/get_MOD07_HEMI.sh $year $(printf "%.3d" $jday)
        rm $DATA_PATH/HEMI/MOD07L2/${year}/MOD07_L2.A${year}$(printf "%.3d" $jday)hemi.txt
        cd $HOME
        $SCRIPTS_PATH/get_IMERG_HDF5.sh
    
        #####  Subset IMERG HDF5 and calculate durations/sums  #####
        $SCRIPTS_PATH/HEMI_IMERG_manual_GrADs.sh 
    
        cd $HOME
        #####  Run fire risk models  #####
        $SCRIPTS_PATH/HEMI_MOD07_IMERG_FIRECAST_manual_nas_gdal_win2_plevels_lm.sh
#         tput bel
    done

else

    for (( jday = $beginjday; jday <= $endjday; jday++ ))
    do
        ### prevyear is the year of the previous julian day.                                  ###
        ### which is the same as $year unless beginjday is 001.                               ###
        export set prevyear=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%Y)
        export set jday=$jday
        export set prevjday=$( date -d "$year-01-01 +$(( $jday - 2 ))days" +%-j)
        
        #####  convert jday to YYYY-MM-DD  #####
        export set fulldate=$( date -d "$year-01-01 +$(( $jday - 1 ))days" +%Y-%m-%d )
    
        printf "\n***** HEMI Fire Risk for $fulldate *****\n\n"
        
        cd $HOME/BIN
        #####  Download MOD07 and IMERG data for jday  #####
        java -jar MOD7DownloaderHEMI.jar $DATA_PATH/HEMI/MOD07L2/${year}/MOD07_L2.A${year}$(printf "%.3d" $jday)hemi.txt ${fulldate}
        $SCRIPTS_PATH/get_MOD07_HEMI.sh $year $(printf "%.3d" $jday)
        $SCRIPTS_PATH/rem_MOD07_HEMI.sh $year $(printf "%.3d" $jday)
#         rm $DATA_PATH/HEMI/MOD07L2/${year}/MOD07_L2.A${year}$(printf "%.3d" $jday)hemi.txt
        cd $HOME
        $SCRIPTS_PATH/get_IMERG_HDF5.sh
    
        #####  Subset IMERG HDF5 and calculate durations/sums  #####
        $SCRIPTS_PATH/HEMI_IMERG_manual_GrADs.sh 

    
        cd $HOME
        #####  Run fire risk models  #####
        $SCRIPTS_PATH/HEMI_MOD07_IMERG_FIRECAST_manual_nas_gdal_win2_plevels_lm.sh

#         tput bel
    done
cat /proc/cpuinfo
cat /proc/meminfo
fi

printf "\n\n`env`\n\n"
#aws ec2 stop-instances --instance-ids i-f1c40824
#aws s3 cp /firecast/missingIMERG.txt s3://suitability-mapper/fire-risk/missingIMERG.txt
#shutdown -P +5
echo
echo "`date`"

if [ ! -f "${DATA_PATH}/HEMI/MODOUT/FR_INDEX/${prevyear}/${prevyear}$(printf "%.3d" $prevjday)_HEMI_FireRisk.tif" ]
then
    echo "${DATA_PATH}/HEMI/MODOUT/FR_INDEX/${prevyear}/${prevyear}$(printf "%.3d" $prevjday)_HEMI_FireRisk.tif"
    cat ${HOME}/AlertHeader.txt ${HOME}/LatestConsoleOutput.log > AlertEmail.txt
    ssmtp sotergre@msu.edu < AlertEmail.txt
    exit 0
fi
if [ ! -f "${DATA_PATH}/HEMI/MODOUT/DSLR/${prevyear}/DSLR_${prevyear}$(printf "%.3d" $prevjday)_HEMI.tif" ]
then
    echo "${DATA_PATH}/HEMI/MODOUT/DSLR/${prevyear}/DSLR_${prevyear}$(printf "%.3d" $prevjday)_HEMI.tif"
    cat ${HOME}/AlertHeader.txt ${HOME}/LatestConsoleOutput.log > AlertEmail.txt
    ssmtp sotergre@msu.edu < AlertEmail.txt
    exit 0
fi
if [ ! -f "${DATA_PATH}/HEMI/MODOUT/FR_INDEX/${prevyear}/${prevyear}$(printf "%.3d" $prevjday)_HEMI_FireRisk.tif" ]
then
    echo "${DATA_PATH}/HEMI/MODOUT/FR_INDEX/${prevyear}/${prevyear}$(printf "%.3d" $prevjday)_HEMI_FireRisk.tif"
    cat ${HOME}/AlertHeader.txt ${HOME}/LatestConsoleOutput.log > AlertEmail.txt
    ssmtp sotergre@msu.edu < AlertEmail.txt
    exit 0
fi
if [ ! -f "${DATA_PATH}/HEMI/MODOUT/DSLR/${prevyear}/DSLR_${prevyear}$(printf "%.3d" $prevjday)_HEMI.tif" ]
then
    echo "${DATA_PATH}/HEMI/MODOUT/DSLR/${prevyear}/DSLR_${prevyear}$(printf "%.3d" $prevjday)_HEMI.tif"
    cat ${HOME}/AlertHeader.txt ${HOME}/LatestConsoleOutput.log > AlertEmail.txt
    ssmtp sotergre@msu.edu < AlertEmail.txt
    exit 0
fi

cat ${HOME}/AlertHeader.txt ${HOME}/Success > AlertEmail.txt
ssmtp sotergre@msu.edu < AlertEmail.txt

# tput bel
# tput bel

exit 0
