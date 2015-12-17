function main ( args )

'rc = gsfallow("on")'

xmin = -82
xmax = -54
ymin = -28
ymax = 13

file = args
'open /firecast/DATA/gtifheader/IMERG.ctl 'file

'set lon 'xmin' 'xmax
'set lat 'ymin' 'ymax

'save -v Calp -u -9999.9 -o 'file'.cal'
'close 1'


return
exec main

