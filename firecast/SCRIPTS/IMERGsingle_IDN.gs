function main ( args )

'rc = gsfallow("on")'

xmin = 94.5
xmax = 143.5
ymin = -11.0
ymax = 7.5

file = args
'open /firecast/DATA/IDN/gtifheader/IMERG.ctl 'file

'set lon 'xmin' 'xmax
'set lat 'ymin' 'ymax

'save -v Calp -u -9999.9 -o 'file'.cal'
'close 1'


return
exec main

