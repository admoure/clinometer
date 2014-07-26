#!/bin/bash

BASE_USB="/media/CAM/datalogger/inclinometro"
BASE_PI="/home/pi/soft_inclinometro"


ls -lr $BASE_USB/encurso/*.txt | awk 'BEGIN{counter=0}
			{if (counter > 0){printf ("%s\n",$NF) > "'$BASE_PI'/filestomove.txt"}counter++}'

ls -lr $BASE_USB/encursoc/*.txt | awk 'BEGIN{counter=0}
			{if (counter > 0){printf ("%s\n",$NF) >> "'$BASE_PI'/filestomove.txt"}counter++}'

cat $BASE_PI/filestomove.txt | awk 'BEGIN{}
				{printf("mv %s '$BASE_USB'/original/\n",$NF)}' | bash


