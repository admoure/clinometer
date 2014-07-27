#!/bin/bash

########################################
#############instalador#################
########################################

echo "mkdir /home/pi/soft_inclinometro/"
echo "cp ./soft_inclinometro/*.* /home/pi/soft_inclinometro/"
echo "cd /home/pi/soft_inclinometro/"
echo "chmod 777 /home/pi/soft_inclinometro/*.*"
echo "gcc -o main2 main2.c ad7706.c -l bcm2835"
echo "gcc -o ajuste ajuste.c ad7706.c -l bcm2835"
echo "cd -"
echo "cp ./www/*.* /var/www/"
echo "chmod 777 /var/www/*.*"
echo "cat \"www-data ALL=(root) NOPASSWD: /var/www/incli_graph_local.sh\" >> /etc/sudoers"
echo "crontab ./varios/cron.txt"
