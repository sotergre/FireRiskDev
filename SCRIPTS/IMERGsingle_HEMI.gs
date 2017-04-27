function main ( args )

'rc = gsfallow("on")'

xmin = -180
xmax = 180
ymin = -27
ymax = 13

file = args
'open /firecast/DATA/HEMI/gtifheader/IMERG.ctl 'file

'set lon 'xmin' 'xmax
'set lat 'ymin' 'ymax
say 'q dims'
'save -v Calp -u -9999.9 -o 'file'.cal'
'close 1'


return
exec main

