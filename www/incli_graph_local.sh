#!/bin/bash
# NUMERO_DIAS=5
BASE=/home/pi/soft_inclinometro
BASE_USB=/media/CAM/datalogger/inclinometro
BASE_GRAPH=/var/www

IN_RANGE=$BASE/XYscale.txt
OUT=inclilast.dat
OUTC=inclilastv.dat
OUT_GRAPH=clinoxy.png
OUT_GRAPH2=clinotemp.png
OUT_GRAPH3=3Dview.png


if [ -z $1 ]
then

tiempo=$(cat $IN_RANGE | awk 'BEGIN{FS="="}{print $2}END{}')
set -- $tiempo
echo $5
NUMERO_DIAS=$5

firgas="cat "

for ((i=NUMERO_DIAS;i>-1;i--))
do
	file=`date +%y%02m%02d --date='-'$i' day'`
	year=$(date +%Y --date='-'$i' day')
	month=$(date +%02m --date='-'$i' day')
	firgas="$firgas $BASE_USB/$year/$month/c$file.txt"
done
#file=`date +%y%02m%02d --date='0 day'`
firgas="$firgas > $BASE/$OUT"
echo $firgas | bash

else
	cat $1 > $BASE/$OUT
fi

rango=$(cat $IN_RANGE | awk 'BEGIN{FS="="}{print $2}END{}')
set -- $rango
echo $1 $2 $3 $4
ymax=$1
ymin=$2
y2max=$3
y2min=$4


# BUSCAMOS EL ULTIMO DATO

DATO=$(tail -1 $BASE/$OUT)
echo $DATO
set -- $DATO
alfa=$7"e-6"
beta=$8"e-6"
echo "alfa ="$alfa" beta ="$beta

#REPRESENTAMOS LOS DIAS
echo "reset" > $BASE/gnu_script.gnu
echo "set term dumb" >> $BASE/gnu_script.gnu
echo "plot \"$BASE/$OUT\" using 1:7, \"$BASE/$OUT\" using 1:8 axes x1y2" >> $BASE/gnu_script.gnu
echo "set terminal png truecolor size 1024,480" >> $BASE/gnu_script.gnu
echo "set output \"$BASE_GRAPH/$OUT_GRAPH\"" >> $BASE/gnu_script.gnu
echo "set timefmt \"%y %m %d %H %M %S\"" >> $BASE/gnu_script.gnu
echo "set autoscale xy" >> $BASE/gnu_script.gnu
echo "set xdata time" >> $BASE/gnu_script.gnu
echo "set format x \"%H:%M\\n%d/%m/%y\"" >> $BASE/gnu_script.gnu
echo "set grid xtics ytics" >> $BASE/gnu_script.gnu
echo "set xtics font \"Times-Roman, 10\"" >> $BASE/gnu_script.gnu
echo "set ytics nomirror" >> $BASE/gnu_script.gnu
echo "set ylabel \"X (urad)\" textcolor rgb \"red\"" >> $BASE/gnu_script.gnu
echo "set y2label \"Y (urad)\" textcolor rgb \"green\"" >> $BASE/gnu_script.gnu
echo "set y2tics nomirror" >> $BASE/gnu_script.gnu
#echo "ymax = GPVAL_Y2_MAX >= GPVAL_Y_MAX ? GPVAL_Y2_MAX : GPVAL_Y_MAX;" >> $BASE/gnu_script.gnu
#echo "ymin = GPVAL_Y2_MIN <= GPVAL_Y_MIN ? GPVAL_Y2_MIN : GPVAL_Y_MIN;" >> $BASE/gnu_script.gnu
echo "set yrange["$ymin":"$ymax"]" >> $BASE/gnu_script.gnu
echo "set y2range["$y2min":"$y2max"]" >> $BASE/gnu_script.gnu
echo "plot \"$BASE/$OUT\" using 1:7 title \"X\" with lines linecolor rgb \"red\", \"$BASE/$OUT\" using 1:8 title \"Y\" with lines linecolor rgb \"green\" axes x1y2" >> $BASE/gnu_script.gnu

echo "reset" >> $BASE/gnu_script.gnu
echo "clear" >> $BASE/gnu_script.gnu
echo "set terminal png truecolor size 1024,480" >> $BASE/gnu_script.gnu
echo "set output \"$BASE_GRAPH/$OUT_GRAPH2\"" >> $BASE/gnu_script.gnu
echo "set timefmt \"%y %m %d %H %M %S\"" >> $BASE/gnu_script.gnu
echo "set autoscale xy" >> $BASE/gnu_script.gnu
echo "set xdata time" >> $BASE/gnu_script.gnu
echo "set format x \"%H:%M\\n%d/%m/%y\"" >> $BASE/gnu_script.gnu
echo "set grid xtics ytics" >> $BASE/gnu_script.gnu
echo "set xtics font \"Times-Roman, 10\"" >> $BASE/gnu_script.gnu
echo "set xlabel \"Se representan los últimos "$NUMERO_DIAS" dias\"" >> $BASE/gnu_script.gnu
echo "set y2label \"Temperatura (Celsius)\" textcolor rgb \"blue\"" >> $BASE/gnu_script.gnu
echo "unset y2tics" >> $BASE/gnu_script.gnu
echo "set ylabel \"Temperatura (Celsius)\" textcolor rgb \"blue\"" >> $BASE/gnu_script.gnu
echo "set y2tics" >> $BASE/gnu_script.gnu
echo "set autoscale xy" >> $BASE/gnu_script.gnu
echo "plot \"$BASE/$OUT\" using 1:9 title \"Temperatura\" with lines linecolor rgb \"blue\" axes x1y2" >> $BASE/gnu_script.gnu

chmod 777 $BASE/gnu_script.gnu

gnuplot $BASE/gnu_script.gnu

echo "set term dumb" > $BASE/gnu_script2.gnu
echo "set output \"$BASE_GRAPH/$OUT_GRAPH3\"" >> $BASE/gnu_script2.gnu
echo "unset xtics" >> $BASE/gnu_script2.gnu
echo "unset ytics" >> $BASE/gnu_script2.gnu
echo "unset ztics" >> $BASE/gnu_script2.gnu
echo "set xzeroaxis" >> $BASE/gnu_script2.gnu
echo "set yzeroaxis" >> $BASE/gnu_script2.gnu
echo "set zzeroaxis" >> $BASE/gnu_script2.gnu
echo "set border 0" >> $BASE/gnu_script2.gnu
echo "set label \"X+\" at 2.5,0,0" >> $BASE/gnu_script2.gnu
echo "set label \"Y+\" at 0,2.5,0" >> $BASE/gnu_script2.gnu
echo "set angle radians" >> $BASE/gnu_script2.gnu
echo "alfa = "$alfa >> $BASE/gnu_script2.gnu
echo "beta = "$beta >> $BASE/gnu_script2.gnu
echo "set xrange [-1:1]" >> $BASE/gnu_script2.gnu
echo "set yrange [-1:1]" >> $BASE/gnu_script2.gnu
echo "maximo=abs(sin("$alfa")+sin("$beta"))" >> $BASE/gnu_script2.gnu
echo "set zrange [-maximo:maximo]" >> $BASE/gnu_script2.gnu
echo "set xyplane at 0" >> $BASE/gnu_script2.gnu
echo "set nokey" >> $BASE/gnu_script2.gnu

echo "splot \"/home/pi/soft_inclinometro/sero.txt\" using 1:2:3 notitle with points" >> $BASE/gnu_script2.gnu
echo "set terminal png truecolor size 1024,300" >> $BASE/gnu_script2.gnu
echo "set multiplot layout 1,3" >> $BASE/gnu_script2.gnu

echo "set arrow from 1,1,0 to -1,1,0 nohead" >> $BASE/gnu_script2.gnu
echo "set arrow from -1,1,0 to -1,-1,0 nohead" >> $BASE/gnu_script2.gnu
echo "set arrow from -1,-1,0 to 1,-1,0 nohead" >> $BASE/gnu_script2.gnu
echo "set arrow from 1,-1,0 to 1,1,0 nohead" >> $BASE/gnu_script2.gnu

echo "set arrow from 0,0,0 to 2,0,0 linecolor rgb \"red\"" >> $BASE/gnu_script2.gnu
echo "set arrow from 0,0,0 to 0,2,0 linecolor rgb \"green\"" >> $BASE/gnu_script2.gnu

echo "set arrow from 0,0,0 to cos("$alfa"),0,sin("$alfa") nohead lw 4 lt 3" >> $BASE/gnu_script2.gnu
echo "set arrow from 0,0,0 to 0,cos("$beta"),sin("$beta") nohead lw 4 lt 3" >> $BASE/gnu_script2.gnu
echo "set arrow from 0,0,0 to -cos("$alfa"),0,-sin("$alfa") nohead lw 4 lt 3" >> $BASE/gnu_script2.gnu
echo "set arrow from 0,0,0 to 0,-cos("$beta"),-sin("$beta") nohead lw 4 lt 3" >> $BASE/gnu_script2.gnu

echo "set arrow from cos("$alfa"),cos("$beta"),sin("$alfa")+sin("$beta") to -cos("$alfa"),cos("$beta"),-sin("$alfa")+sin("$beta") nohead lw 4 lt 3" >> $BASE/gnu_script2.gnu
echo "set arrow from cos("$alfa"),cos("$beta"),sin("$alfa")+sin("$beta") to cos("$alfa"),-cos("$beta"),sin("$alfa")-sin("$beta") nohead lw 4 lt 3" >> $BASE/gnu_script2.gnu
echo "set arrow from cos("$alfa"),-cos("$beta"),sin("$alfa")-sin("$beta") to -cos("$alfa"),-cos("$beta"),-sin("$alfa")-sin("$beta") nohead lw 4 lt 3" >> $BASE/gnu_script2.gnu
echo "set arrow from -cos("$alfa"),-cos("$beta"),-sin("$alfa")-sin("$beta") to -cos("$alfa"),cos("$beta"),-sin("$alfa")+sin("$beta") nohead lw 4 lt 3" >> $BASE/gnu_script2.gnu


echo "set label font \"Times-Roman, 9\" sprintf (\"X = %.2f urad\nY = %.2f urad\","$alfa"/1e-6,"$beta"/1e-6) at 0,0,-maximo*2" >> $BASE/gnu_script2.gnu

echo "set view 73,49" >> $BASE/gnu_script2.gnu
echo "replot" >> $BASE/gnu_script2.gnu
echo "set view 73,229" >> $BASE/gnu_script2.gnu
echo "replot" >> $BASE/gnu_script2.gnu
echo "set view 64,135" >> $BASE/gnu_script2.gnu
echo "replot" >> $BASE/gnu_script2.gnu
echo "unset multiplot" >> $BASE/gnu_script2.gnu

chmod 777 $BASE/gnu_script2.gnu
gnuplot $BASE/gnu_script2.gnu





