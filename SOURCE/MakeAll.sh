#! /bin/bash

make -f Make.averagemonths_3B43PP_GLOBAL_V7
make -f Make.convert1000pto100p
make -f Make.convertKx10toC
make -f Make.convert_sint2uchar
make -f Make.dursum_3B42PP_AMZ
make -f Make.dursum_3B42PP_AMZEX_win2_gdal
make -f Make.dursum_3B42RT6_AMZ
make -f Make.dursum_3B42RT7_AMZ
make -f Make.dursum_3B42RT_AMZ
make -f Make.dursum_3B42RT_AMZEX_32_win2
make -f Make.dw_interp_MOD07_SFT
make -f Make.dw_interp_MOD07_TP
make -f Make.firerisk_index
make -f Make.firerisk_index_gdal
make -f Make.fmodel_100hr_1000hr
make -f Make.fmodel_10hr
make -f Make.HDF_QC
make -f Make.interpolate_MOD07_SFT
make -f Make.interpolate_MOD07_TP
make -f Make.Keetch_Byram
make -f Make.Keetch_Byram_SI
make -f Make.MOD35_readQC
make -f Make.rescale_dslr
make -f Make.rhumid_at_dpt
make -f Make.sum_3B42PP_GLOBAL_V7
make -f Make.sum_3B42RT_GMT

exit

