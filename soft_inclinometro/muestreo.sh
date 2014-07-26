#!/bin/bash
BASE_PI="\/home\/pi\/soft_inclinometro\/"
FILE="main2"
TEMP="new_file.txt"
CRONB="cron_backup.txt"

crontab -l > new_file.txt
if [ $1 -eq 0 ]
then
	sed '23s/.*/\#  *\/'$1' * * * * '$BASE_PI''$FILE' /' $TEMP > $CRONB
else
	sed '23s/.*/  *\/'$1' * * * * '$BASE_PI''$FILE' /' $TEMP > $CRONB
fi
crontab cron_backup.txt

