#!/bin/bash

anio=$(date -u +%Y)
aniop=$(date -u +%y)
mes=$(date -u +%m)
dia=$(date -u +%d)
hora=$(date -u +%H)
minuto=$(date -u +%M)

BASE="/home/pi/soft_inclinometro"
BASE_USB="/media/CAM/datalogger/inclinometro"

hola=$(cat $BASE/configura.txt | awk 'BEGIN{FS="="}{printf("%s ",$2)}END{}')
set -- $hola

#echo "file "$aniop$mes$dia.cab "en" $BASE_USB/$anio/$mes
echo "#" $hora:$minuto "#" >> $BASE_USB/$anio/$mes/$aniop$mes$dia.cab
echo "name:"$1 >> $BASE_USB/$anio/$mes/$aniop$mes$dia.cab 
echo "serial:"$2 >> $BASE_USB/$anio/$mes/$aniop$mes$dia.cab
echo "channels:"$3 $4 $5 >> $BASE_USB/$anio/$mes/$aniop$mes$dia.cab
echo "interval:"$6"min" >> $BASE_USB/$anio/$mes/$aniop$mes$dia.cab
echo "gain:"$7 >> $BASE_USB/$anio/$mes/$aniop$mes$dia.cab
echo "filter:"$8 >> $BASE_USB/$anio/$mes/$aniop$mes$dia.cab
echo "lat:"$9 "lon:"${10} "alt:"${11} >> $BASE_USB/$anio/$mes/$aniop$mes$dia.cab
echo "server:"${13} >> $BASE_USB/$anio/$mes/$aniop$mes$dia.cab
echo "port:"${14} >> $BASE_USB/$anio/$mes/$aniop$mes$dia.cab
echo "folder:"${15} >> $BASE_USB/$anio/$mes/$aniop$mes$dia.cab




