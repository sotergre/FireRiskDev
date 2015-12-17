#! /bin/zsh

for month in jan feb mar apr may jun jul aug sep oct nov dec
do
/FIRERISK/BIN/averagemonths_3B42PP_GLOBAL_V7 1998 2012 $month /FIRERISK/DATA/GLOBAL/3B42PP/
done