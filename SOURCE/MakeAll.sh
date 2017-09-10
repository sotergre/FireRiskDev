#! /bin/bash
cd include
rm *.o
gcc -g -pedantic -ansi -c -o allocate_binary.o allocate_binary.c
gcc -g -pedantic -ansi -c -o read_binary.o read_binary.c
gcc -g -pedantic -ansi -c -o write_binary.o write_binary.c
gcc -g -pedantic -ansi -c -o read_binary_fmodel.o read_binary_fmodel.c
gcc -g -pedantic -ansi -c -o byteswap_3B42RT.o byteswap_3B42RT.c
cd ..
make -f Make.convert1000pto100p
make -f Make.convertKx10toC
make -f Make.convert_sint2uchar
make -f Make.days_since_last_rain_AMZEX
make -f Make.dursum_3B42PP_AMZ
make -f Make.dursum_3B42PP_AMZEX_win2_gdal
make -f Make.dursum_3B42RT6_AMZ
make -f Make.dursum_3B42RT7_AMZ
make -f Make.dursum_3B42RT_AMZ
make -f Make.dursum_3B42RT_AMZEX_32_win2
make -f Make.dursum_dursum_IMERGHDF_AMZ
make -f Make.dursum_dursum_IMERGHDF_AMZ_GS
make -f Make.dursum_dursum_IMERGHDF_IDN
make -f Make.dursum_IMERG_AMZ_GMT
make -f Make.dursum_IMERGHDF_AMZ
make -f Make.dursum_IMERGHDF_IDN
make -f Make.dursum_IMERGHDF_HEMI
make -f Make.dw_interp_MOD07_SFT
make -f Make.dw_interp_MOD07_TP
make -f Make.firerisk_index
make -f Make.firerisk_index_gdal
make -f Make.firerisk_index_nas_v2
make -f Make.firerisk_index_nas_v2_idn
make -f Make.fmodel_100hr_1000hr
make -f Make.fmodel_100hr_1000hr_nas_v2
make -f Make.fmodel_100hr_1000hr_nas_v2_IMERG
make -f Make.fmodel_100hr_1000hr_nas_v2_IMERG_idn
make -f Make.fmodel_10hr
make -f Make.fmodel_10hr_nas_v2
make -f Make.fmodel_10hr_nas_v2_IMERG
make -f Make.HDF_QC
make -f Make.interpolate_MOD07_SFT
make -f Make.interpolate_MOD07_TP
make -f Make.interpolate_RH_temporal_nas
make -f Make.interpolate_RH_temporal_nas_16day
make -f Make.interpolate_RH_temporal_nas_32day
make -f Make.interpolate_RH_temporal_nas_idn
make -f Make.interpolate_SFT_temporal_nas
make -f Make.interpolate_SFT_temporal_nas_16day
make -f Make.interpolate_SFT_temporal_nas_32day
make -f Make.interpolate_SFT_temporal_nas_idn
make -f Make.Keetch_Byram
make -f Make.Keetch_Byram_SI
make -f Make.MOD35_readQC
make -f Make.rescale_dslr
make -f Make.rescale_dslr_nas
make -f Make.rescale_dslr_nas_idn
make -f Make.rhumid_at_dpt
make -f Make.rhumid_at_dpt_nas
make -f Make.rhumid_at_dpt_nas_plevels
make -f Make.rhumid_st_dpt_nas_plevels
make -f Make.rhumid_st_dpt_nas_plevels_idn
make -f Make.sum_3B42PP_GLOBAL_V7
make -f Make.sum_3B42RT_GMT
make -f Make.rhumid_st_dpt_nas_plevels_HEMI
make -f Make.interpolate_SFT_temporal_nas_32day_HEMI
make -f Make.interpolate_RH_temporal_nas_32day_HEMI
make -f Make.fmodel_100hr_1000hr_nas_v2_IMERG_HEMI

rm *.o

javac -d "$HOME/SOURCE/MOD7DownloaderHEMI" -cp "$HOME/SOURCE/MOD7DownloaderHEMI" ${HOME}/SOURCE/MOD7Data.java
    
jar -cvfm "$HOME/SOURCE/MOD7DownloaderHEMI.jar" "$HOME/SOURCE/MOD7DownloaderHEMI/META-INF/MANIFEST.MF" -C "$HOME/SOURCE/MOD7DownloaderHEMI/" .

mv ${HOME}/SOURCE/MOD7DownloaderHEMI.jar ${HOME}/BIN/MOD7DownloaderHEMI.jar

exit 0




















