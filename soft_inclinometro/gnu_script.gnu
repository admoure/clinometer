reset
set term dumb
plot "/home/pi/soft_inclinometro/inclilast.dat" using 1:7, "/home/pi/soft_inclinometro/inclilast.dat" using 1:8 axes x1y2
set terminal png truecolor size 1024,480
set output "/var/www/clinoxy.png"
set timefmt "%y %m %d %H %M %S"
set autoscale xy
set xdata time
set format x "%H:%M\n%d/%m/%y"
set grid xtics ytics
set xtics font "Times-Roman, 10"
set ytics nomirror
set ylabel "X (urad)" textcolor rgb "red"
set y2label "Y (urad)" textcolor rgb "green"
set y2tics nomirror
set yrange[100:200]
set y2range[175:275]
plot "/home/pi/soft_inclinometro/inclilast.dat" using 1:7 title "X" with lines linecolor rgb "red", "/home/pi/soft_inclinometro/inclilast.dat" using 1:8 title "Y" with lines linecolor rgb "green" axes x1y2
reset
clear
set terminal png truecolor size 1024,480
set output "/var/www/clinotemp.png"
set timefmt "%y %m %d %H %M %S"
set autoscale xy
set xdata time
set format x "%H:%M\n%d/%m/%y"
set grid xtics ytics
set xtics font "Times-Roman, 10"
set xlabel "Se representan los últimos 20 dias"
set y2label "Temperatura (Celsius)" textcolor rgb "blue"
unset y2tics
set ylabel "Temperatura (Celsius)" textcolor rgb "blue"
set y2tics
set autoscale xy
plot "/home/pi/soft_inclinometro/inclilast.dat" using 1:9 title "Temperatura" with lines linecolor rgb "blue" axes x1y2
