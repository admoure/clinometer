#!/bin/bash

########################################
#############instalador#################
########################################

mkdir /home/pi/soft_inclinometro/
cp ./soft_inclinometro/*.* /home/pi/soft_inclinometro/
cd /home/pi/soft_inclinometro/
chmod 777 /home/pi/soft_inclinometro/*.*
gcc -o main2 main2.c ad7706.c -l bcm2835
gcc -o ajuste ajuste.c ad7706.c -l bcm2835
cd -
cp ./www/*.* /var/www/
chmod 777 /var/www/*.*
echo "www-data ALL=(root) NOPASSWD: /var/www/incli_graph_local.sh" > ./clino
chmod 0440 clino
mv clino /etc/sudoers.d
crontab ./varios/cron.txt
