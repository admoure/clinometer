#!/bin/bash

check=$(cat /home/pi/soft_inclinometro/configura.txt | awk 'BEGIN{FS="="}{

	if ($1 == "SERVERCHECK")
	{
		if ($2 == "ON")
		{
			print "true";
		}
		else
		{
			print "false";
		}
	}
}END{}')

server=$(cat /home/pi/soft_inclinometro/configura.txt | awk 'BEGIN{FS="="}{

	if ($1 == "SERVER" && "'$check'" == "true")
	{
		print $2;
	}
}END{}')

port=$(cat /home/pi/soft_inclinometro/configura.txt | awk 'BEGIN{FS="="}{

	if ($1 == "PORT" && "'$check'" == "true")
	{
		print $2;
	}
}END{}')

folder=$(cat /home/pi/soft_inclinometro/configura.txt | awk 'BEGIN{FS="="}{

	if ($1 == "FOLDER" && "'$check'" == "true")
	{
		print $2;
	}
}END{}')


#rsync -zrpvc -e 'ssh -p '$port'' --ignore-existing /media/CAM/datalogger/inclinometro/ svvcog@$server:$folder
echo "rsync --update -razv 'ssh -p '$port'' /media/CAM/datalogger/inclinometro/ svvcog@$server:$folder"
rsync --update --timeout=30 -razvP -e 'ssh -p '$port'' /media/CAM/datalogger/inclinometro/ svvcog@$server:$folder

