#!/bin/bash
IP='65.254.250.102'
#IP='88.2.245.55'
#USER='ign_grav_izana'
#PASS='svv-teide'
USER='admoure'
PASS='&admoure11'
DIR_FILES='/encurso'
BASE='/media/CAM/datalogger/gravimetro'
BASE_PI='/home/pi/soft_gravimetro'

#se listan los txt del directorio y se crea un script del lftp

ls -lt $BASE/$DIR_FILES/*.txt | awk 'BEGIN{printf "debug -o debug.txt\n" > "'$BASE_PI'/ftp_script.ftp"; 
			printf "open '$IP'\n" >> "'$BASE_PI'/ftp_script.ftp"; 
			printf "user '$USER' \"'$PASS'\"\n" >> "'$BASE_PI'/ftp_script.ftp"
			printf "cd gravimetro2\n" >> "'$BASE_PI'/ftp_script.ftp"}
			{printf "put %s\n",$NF >> "'$BASE_PI'/ftp_script.ftp"} 
			END{printf "bye\n" >> "'$BASE_PI'/ftp_script.ftp"}' | bash

chmod 777 $BASE_PI/ftp_script.ftp
lftp -f $BASE_PI/ftp_script.ftp

#Ahora se mueven todos los ficheros del directorio encurso y los cambia de carpeta (original)

ls -lt $BASE/$DIR_FILES/*.txt | awk 'BEGIN{ FS="/";variable=0;
			printf "mkdir '$BASE'/original\n" }
			{if (variable!=0) printf "mv '$BASE'/'$DIR_FILES'/%s '$BASE'/original/%s\n",$NF,$NF}
			{variable++}' | bash

