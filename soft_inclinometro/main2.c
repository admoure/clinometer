#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bcm2835.h>
#include "ad7706.h"

#define RESET_AD RPI_V2_GPIO_P1_07
#define LED	 RPI_V2_GPIO_P1_15

int flag_cabecera=0;	//flag que mira si ha habido algún cambio en la cabecera
float result1,result2,result3;
char medida1[12],medida2[12],medida3[12];
int loop;
float sol1[50],sol2[50],sol3[50];

unsigned char datoh,datol,temp,rerun;
unsigned int dato,sumatorio;
unsigned char caracteres[128];

char msg[128];
char base[256]={"/media/CAM/datalogger/inclinometro"};
char file_configura[256]={"/home/pi/soft_inclinometro/configura.txt"};
char file_configura_ant[256]={"/home/pi/soft_inclinometro/configurab.txt"};
char file_calibra[256]={"/home/pi/soft_inclinometro/calib.txt"};
char base_pi[256]={"/home/pi/soft_inclinometro"};
FILE *fc;
FILE *fcb;
FILE *fp;
FILE *fcali;

unsigned char lectura[10];
unsigned char regad;
char horas[3],minutos[3],segundos[3],dia[3],mes[3],anio[3],anno[5],monthl[4];
int hours,minutes,seconds,year,day,month,yearg;

char chan1[4],chan2[4],chan3[4],muest[6],name[10],nserie[10];
char gain[4],filter[4],lat[16],lon[16],alt[16];
char chan1b[4],chan2b[4],chan3b[4],muestb[6],nameb[10],nserieb[10];
char gainb[4],filterb[4],latb[16],lonb[16],altb[16];
char serverc[4],server[32],port[6],folder[64];
char servercb[4],serverb[32],portb[6],folderb[64];


char aux[32];
int muestreo;

char *ptr;
float va1,vb1,va2,vb2,va3,vb3;

float a1l,b1l,c1l,d1l,e1l,f1l;
float a2l,b2l,c2l,d2l,e2l,f2l;
float a1h,b1h,c1h,d1h,e1h,f1h;
float a2h,b2h,c2h,d2h,e2h,f2h;

float cuentas_a_voltaje_canal1 = 0;
float cuentas_a_voltaje_canal2 = 0;
float cuentas_a_voltaje_canal3 = 0;

float vec1 = 0;
float vec2 = 0;
float vec3 = 0;

float urad_canal1 = 0;
float urad_canal2 = 0;
float urad_canal3 = 0;

char urad_1[16];
char urad_2[16];
char urad_3[16];

int main()
{
	fc = fopen(file_configura,"r");
	if (fc == NULL)
	{
		printf("el fichero no existe\n");
		return 1;
	}
	fscanf(fc,"NAME=%s\n",name);
	fscanf(fc,"NSERIAL=%s\n",nserie);
	fscanf(fc,"CHANNEL1=%s\n",chan1);
	fscanf(fc,"CHANNEL2=%s\n",chan2);
	fscanf(fc,"CHANNEL3=%s\n",chan3);
	fscanf(fc,"MUESTREO=%s\n",muest);
	fscanf(fc,"HIGHGAIN=%s\n",gain);
	fscanf(fc,"FILTER=%s\n",filter);
	fscanf(fc,"LAT=%s\n",lat);
	fscanf(fc,"LON=%s\n",lon);
	fscanf(fc,"ALT=%s\n",alt);

	fscanf(fc,"SERVERCHECK=%s\n",serverc);
	fscanf(fc,"SERVER=%s\n",server);
	fscanf(fc,"PORT=%s\n",port);
	fscanf(fc,"FOLDER=%s\n",folder);

	fclose(fc);

	fcb = fopen(file_configura_ant,"r");
	if (fcb == NULL)
	{
		printf ("cagada");
	}
	else
	{
		fscanf(fcb,"NAME=%s\n",nameb);
		fscanf(fcb,"NSERIAL=%s\n",nserieb);
		fscanf(fcb,"CHANNEL1=%s\n",chan1b);
		fscanf(fcb,"CHANNEL2=%s\n",chan2b);
		fscanf(fcb,"CHANNEL3=%s\n",chan3b);
		fscanf(fcb,"MUESTREO=%s\n",muestb);
		fscanf(fcb,"HIGHGAIN=%s\n",gainb);
		fscanf(fcb,"FILTER=%s\n",filterb);
		fscanf(fcb,"LAT=%s\n",latb);
		fscanf(fcb,"LON=%s\n",lonb);
		fscanf(fcb,"ALT=%s\n",altb);

		fscanf(fcb,"SERVERCHECK=%s\n",servercb);
		fscanf(fcb,"SERVER=%s\n",serverb);
		fscanf(fcb,"PORT=%s\n",portb);
		fscanf(fcb,"FOLDER=%s\n",folderb);
		fclose(fcb);
	}

	//PONEMOS LOS DATOS DEL FICHERO ORIGIANL DE CONFIGURACION

	printf("NOMBRE DE LA ESTACION > %s\n",name);
	printf("NUMERO DE SERIE DEL SENSOR > %s\n",nserie);
	printf("SERVIDOR=%s\n",server);
	printf("PUERTO=%s\n",port);
	printf("CARPETA=%s\n",folder);

	printf("CANAL 1 > %s\n",chan1);
	printf("CANAL 2 > %s\n",chan2);
	printf("CANAL 3 > %s\n",chan3);
	muestreo = atoi(muest);
	printf("muestreo > %d\n",muestreo);
	printf("HIGH GAIN > %s\n",gain);
	printf("FILTRO > %s\n",filter);
	printf("latitud > %s\n",lat);
	printf("longitud > %s\n",lon);
	printf("altitud > %s\n",alt);


	//HABRIMOS EL FICHERO DE CALIBRACION
	fcali = fopen(file_calibra,"r+");
	if (fcali == NULL)
	{
		printf("el fichero no existe\n");
	}
	while(!feof(fcali))
	{
		fgets(caracteres,sizeof(caracteres),fcali);
		ptr = strstr(caracteres,"*CALIBRACION");
		if(ptr)
		{
			printf("%s\n",caracteres);
			fgets(caracteres,sizeof(caracteres),fcali);
			sscanf(caracteres,"vout1=%f*vin %f",&va1,&vb1);
			printf("CANAL 1 A = %f  B = %f\n",va1,vb1);
			fgets(caracteres,sizeof(caracteres),fcali);
			sscanf(caracteres,"vout2=%f*vin %f",&va2,&vb2);
			printf("CANAL 2 A = %f  B = %f\n",va2,vb2);
			fgets(caracteres,sizeof(caracteres),fcali);
			sscanf(caracteres,"vout3=%f*vin %f",&va3,&vb3);
			printf("CANAL 3 A = %f  B = %f\n",va3,vb3);
			fgets(caracteres,sizeof(caracteres),fcali);
			sscanf(caracteres,"y1l=%f*x5 %f*x4 %f*x3 %f*x2 %f*x %f\n",&a1l,&b1l,&c1l,&d1l,&e1l,&f1l);
			printf("%f %f %f %f %f %f\n",a1l,b1l,c1l,d1l,e1l,f1l);
			fgets(caracteres,sizeof(caracteres),fcali);
			sscanf(caracteres,"y2l=%f*x5 %f*x4 %f*x3 %f*x2 %f*x %f\n",&a2l,&b2l,&c2l,&d2l,&e2l,&f2l);
			printf("%f %f %f %f %f %f\n",a2l,b2l,c2l,d2l,e2l,f2l);
			fgets(caracteres,sizeof(caracteres),fcali);
			sscanf(caracteres,"y1h=%f*x5 %f*x4 %f*x3 %f*x2 %f*x %f\n",&a1h,&b1h,&c1h,&d1h,&e1h,&f1h);
			printf("%f %f %f %f %f %f\n",a1h,b1h,c1h,d1h,e1h,f1h);
			fgets(caracteres,sizeof(caracteres),fcali);
			sscanf(caracteres,"y2h=%f*x5 %f*x4 %f*x3 %f*x2 %f*x %f\n",&a2h,&b2h,&c2h,&d2h,&e2h,&f2h);
			printf("%f %f %f %f %f %f\n",a2h,b2h,c2h,d2h,e2h,f2h);
			break;

		}
	}
	fclose(fcali);

	if(!bcm2835_init())
		return 1;

	bcm2835_gpio_fsel(LED,BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(RESET_AD,BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_set(RESET_AD);

	//se habilita el spi
	bcm2835_spi_begin();
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);
	//se habilita el i2c
//	bcm2835_i2c_begin();
//	bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626);
//	bcm2835_i2c_setSlaveAddress(0x50);


	bcm2835_gpio_set(LED);			//COMIENZA LA MEDIDA
	//TOMAMOS LA HORA DE LA MEDIDA//HAY QUE HACER CONTROL DE CALIDAD

//	regad = 0x02;
//	bcm2835_i2c_read_register_rs(&regad,lectura,5);
	fp = popen("date -u +%S","r");
	fgets(segundos,3,fp);
	pclose(fp);
	fp = popen("date -u +%M","r");
	fgets(minutos,3,fp);
	pclose(fp);
	fp = popen("date -u +%H","r");
	fgets(horas,3,fp);
	pclose(fp);
	fp = popen("date -u +%y","r");
	fgets(anio,3,fp);
	pclose(fp);
	fp = popen("date -u +%Y","r");
	fgets(anno,5,fp);
	pclose(fp);
	fp = popen("date -u +%m","r");
	fgets(mes,3,fp);
	pclose(fp);
	fp = popen("date -u +%b","r");
	fgets(monthl,4,fp);
	pclose(fp);
	fp = popen("date -u +%d","r");
	fgets(dia,3,fp);
	pclose(fp);
	seconds=atoi(segundos);
	minutes=atoi(minutos);
	hours=atoi(horas);
	month=atoi(mes);
	year=atoi(anio);
	yearg=atoi(anno);
	day=atoi(dia);

	//reiniciamos conversor

	bcm2835_gpio_clr(RESET_AD);
	bcm2835_delay(100);
	bcm2835_gpio_set(RESET_AD);
	bcm2835_delay(500);
	for (loop=0;loop<50;loop++)
	{
		if (strcmp(chan1,"ON")==0)
		{
			bcm2835_spi_transfer(CLOCK_REGISTER|CHANNEL1|_WRITE);
			bcm2835_spi_transfer(CLKDIS_OFF|CLKDIV_OFF|CLK_ON|HZ50);
			bcm2835_spi_transfer(SETUP_REGISTER|CHANNEL1|_WRITE);
			bcm2835_spi_transfer(SELF_CALIBRATION|GAIN_X1|UNIPOLAR_OP|BUFFER_OFF|FSYNC_OFF);

			//bcm2835_delay(500);
			sumatorio = 0;
			for (rerun=0;rerun<8;rerun++)
			{
				do{
					temp = (COMMUNICATION_REGISTER|_READ|CHANNEL1);
					bcm2835_spi_transfer(temp);
					temp = bcm2835_spi_transfer(0);
					//printf("DRDY -> %x\n",temp);
				}while((temp&0x80)!=0);
				temp = (DATA_REGISTER|_READ|CHANNEL1);
				bcm2835_spi_transfer(temp);
				datoh = bcm2835_spi_transfer(0);
				datol = bcm2835_spi_transfer(0);
		//		printf("%x %x\n",datoh,datol);
				dato = (unsigned int)datoh * 256 + (unsigned int)datol;
				sumatorio = sumatorio + (unsigned int)dato;
			}
			result1 = (float)sumatorio/8.;
			sol1[loop]=result1;

			// cuentas_a_voltaje_canal1 = ((result1 * 2.5)/65535.);
			// printf("cuentas a voltaje canal1 -> %f\n",cuentas_a_voltaje_canal1);
			// vec1 = ((cuentas_a_voltaje_canal1 - vb1)/va1)*1000;
			// printf("voltaje salida sensor canal1 -> %f mv\n",vec1);
			// if(strcmp(gain,"ON")==0)
			// {
				// urad_canal1 = (a1h*vec1*vec1*vec1*vec1*vec1)+(b1h*vec1*vec1*vec1*vec1)+(c1h*vec1*vec1*vec1)+(d1h*vec1*vec1)+(e1h*vec1)+f1h;
			// }
			// else
			// {
				// urad_canal1 = (a1l*vec1*vec1*vec1*vec1*vec1)+(b1l*vec1*vec1*vec1*vec1)+(c1l*vec1*vec1*vec1)+(d1l*vec1*vec1)+(e1l*vec1)+f1l;
			// }
			// printf("RAD_CANAL_1 -> %f urad\n",urad_canal1);
	// // result1 = READ_AD_DATA(DATA_REGISTER|_READ|CHANNEL1);
			// sprintf(medida1,"%.4f",result1);
			// sprintf(urad_1,"%.4f",urad_canal1);
		}
		// else
		// {
			// strcpy(medida1,"?");
			// strcpy(urad_1,"?");
		// }

		if (strcmp(chan2,"ON")==0)
		{
			bcm2835_spi_transfer(CLOCK_REGISTER|CHANNEL2|_WRITE);
			bcm2835_spi_transfer(CLKDIS_OFF|CLKDIV_OFF|CLK_ON|HZ50);
			bcm2835_spi_transfer(SETUP_REGISTER|CHANNEL2|_WRITE);
			bcm2835_spi_transfer(SELF_CALIBRATION|GAIN_X1|UNIPOLAR_OP|BUFFER_OFF|FSYNC_OFF);

			//bcm2835_delay(500);
			sumatorio = 0;
			for (rerun=0;rerun<8;rerun++)
			{
				do{
					temp = (COMMUNICATION_REGISTER|_READ|CHANNEL2);
					bcm2835_spi_transfer(temp);
						temp = bcm2835_spi_transfer(0);
					//printf("DRDY -> %x\n",temp);
				}while((temp&0x80)!=0);
				temp = (DATA_REGISTER|_READ|CHANNEL2);
				bcm2835_spi_transfer(temp);
				datoh = bcm2835_spi_transfer(0);
				datol = bcm2835_spi_transfer(0);
		//		printf("%x %x\n",datoh,datol);
				dato = (unsigned int)datoh * 256 + (unsigned int)datol;
				sumatorio = sumatorio + (unsigned int)dato;
			}
			result2 = (float)sumatorio/8.;
			sol2[loop]=result2;
/* 			cuentas_a_voltaje_canal2 = ((result2 * 2.5)/65535.);
			printf("cuentas a voltaje canal2 -> %f\n",cuentas_a_voltaje_canal2);
			vec2 = ((cuentas_a_voltaje_canal2 - vb2)/va2)*1000;
			printf("voltaje salida sensor canal2 -> %f mv\n",vec2);
			if(strcmp(gain,"ON")==0)
			{
				urad_canal2 = (a2h*vec2*vec2*vec2*vec2*vec2)+(b2h*vec2*vec2*vec2*vec2)+(c2h*vec2*vec2*vec2)+(d2h*vec2*vec2)+(e2h*vec2)+f2h;
			}
			else
			{
				urad_canal2 = (a2l*vec2*vec2*vec2*vec2*vec2)+(b2l*vec2*vec2*vec2*vec2)+(c2l*vec2*vec2*vec2)+(d2l*vec2*vec2)+(e2l*vec2)+f2l;
			}

			printf("RAD_CANAL_2 -> %f urad\n",urad_canal2);


		//	result2 = READ_AD_DATA(DATA_REGISTER|_READ|CHANNEL2);
			sprintf(medida2,"%.4f",result2);
			sprintf(urad_2,"%.4f",urad_canal2);*/

		}
//		else
//		{
//			strcpy(medida2,"?");
//			strcpy(urad_2,"?");
//		} 

		if (strcmp(chan3,"ON")==0)
		{
			bcm2835_spi_transfer(CLOCK_REGISTER|CHANNEL3|_WRITE);
			bcm2835_spi_transfer(CLKDIS_OFF|CLKDIV_OFF|CLK_ON|HZ50);
			bcm2835_spi_transfer(SETUP_REGISTER|CHANNEL3|_WRITE);
			bcm2835_spi_transfer(SELF_CALIBRATION|GAIN_X1|UNIPOLAR_OP|BUFFER_OFF|FSYNC_OFF);

			//bcm2835_delay(500);
			sumatorio = 0;
			for (rerun=0;rerun<8;rerun++)
			{
				do{
					temp = (COMMUNICATION_REGISTER|_READ|CHANNEL3);
					bcm2835_spi_transfer(temp);
					temp = bcm2835_spi_transfer(0);
					//printf("DRDY -> %x\n",temp);
				}while((temp&0x80)!=0);
				temp = (DATA_REGISTER|_READ|CHANNEL3);
				bcm2835_spi_transfer(temp);
				datoh = bcm2835_spi_transfer(0);
				datol = bcm2835_spi_transfer(0);
		//		printf("%x %x\n",datoh,datol);
				dato = (unsigned int)datoh * 256 + (unsigned int)datol;
				//printf("%d\n",dato);
				sumatorio = sumatorio + (unsigned int)dato;
			}
			result3 = (float)sumatorio/8.;
			sol3[loop]=result3;

			// cuentas_a_voltaje_canal3 = ((result3 * 2.5)/65535.);
			// printf("cuentas a voltaje canal3 -> %f\n",cuentas_a_voltaje_canal3);
			// vec3 = ((cuentas_a_voltaje_canal3 - vb3)/va3)*1000;
			// printf("voltaje salida sensor canal3 -> %f mv\n",vec3);
			// urad_canal3 = 0.1*vec3;
			// printf("TEMP_CANAL3 -> %.4f ºC\n",urad_canal3);

		// //	result3 = READ_AD_DATA(DATA_REGISTER|_READ|CHANNEL3);

			// sprintf(medida3,"%.4f",result3);
			// sprintf(urad_3,"%.4f",urad_canal3);
		}
		// else
		// {
			// strcpy(medida3,"?");
			// strcpy(urad_3,"?");
		// }
	}
	result1=0;
	result2=0;
	result3=0;
	for (loop=0;loop<50;loop++)
	{
		result1 = result1 + sol1[loop];
		result2 = result2 + sol2[loop];
		result3 = result3 + sol3[loop];
	}
	result1 = result1/50.;
	result2 = result2/50.;
	result3 = result3/50.;
	
	if (strcmp(chan1,"ON")==0)
	{
		cuentas_a_voltaje_canal1 = ((result1 * 2.5)/65535.);
		printf("cuentas a voltaje canal1 -> %f\n",cuentas_a_voltaje_canal1);
		vec1 = ((cuentas_a_voltaje_canal1 -vb1)/va1)*1000;
		printf("voltaje salida sensor canal1 -> %f mv\n",vec1);
		if(strcmp(gain,"ON")==0)
		{
			urad_canal1 = (a1h*vec1*vec1*vec1*vec1*vec1)+(b1h*vec1*vec1*vec1*vec1)+(c1h*vec1*vec1*vec1)+(d1h*vec1*vec1)+(e1h*vec1)+f1h;
		}
		else
		{
			urad_canal1 = (a1l*vec1*vec1*vec1*vec1*vec1)+(b1l*vec1*vec1*vec1*vec1)+(c1l*vec1*vec1*vec1)+(d1l*vec1*vec1)+(e1l*vec1)+f1l;
		}
		printf("RAD_CANAL_1 -> %f urad\n",urad_canal1);
		sprintf(medida1,"%.4f",result1);
		sprintf(urad_1,"%.4f",urad_canal1);
	}
	else
	{
		strcpy(medida1,"?");
		strcpy(urad_1,"?");
	}
	
	if (strcmp(chan2,"ON")==0)
	{
		cuentas_a_voltaje_canal2 = ((result2 * 2.5)/65535.);
		printf("cuentas a voltaje canal2 -> %f\n",cuentas_a_voltaje_canal2);
		vec2 = ((cuentas_a_voltaje_canal2 - vb2)/va2)*1000;
		printf("voltaje salida sensor canal2 -> %f mv\n",vec2);
		if(strcmp(gain,"ON")==0)
		{
			urad_canal2 = (a2h*vec2*vec2*vec2*vec2*vec2)+(b2h*vec2*vec2*vec2*vec2)+(c2h*vec2*vec2*vec2)+(d2h*vec2*vec2)+(e2h*vec2)+f2h;
		}
		else
		{
			urad_canal2 = (a2l*vec2*vec2*vec2*vec2*vec2)+(b2l*vec2*vec2*vec2*vec2)+(c2l*vec2*vec2*vec2)+(d2l*vec2*vec2)+(e2l*vec2)+f2l;
		}

		printf("RAD_CANAL_2 -> %f urad\n",urad_canal2);

		sprintf(medida2,"%.4f",result2);
		sprintf(urad_2,"%.4f",urad_canal2);

	}
	else
	{
		strcpy(medida2,"?");
		strcpy(urad_2,"?");
	}
	
	if (strcmp(chan3,"ON")==0)
	{
	
		cuentas_a_voltaje_canal3 = ((result3 * 2.5)/65535.);
		printf("cuentas a voltaje canal3 -> %f\n",cuentas_a_voltaje_canal3);
		vec3 = ((cuentas_a_voltaje_canal3 - vb3)/va3)*1000;
		printf("voltaje salida sensor canal3 -> %f mv\n",vec3);
		urad_canal3 = 0.1*vec3;
		printf("TEMP_CANAL3 -> %.4f ºC\n",urad_canal3);

		sprintf(medida3,"%.4f",result3);
		sprintf(urad_3,"%.4f",urad_canal3);
	}
	else
	{
		strcpy(medida3,"?");
		strcpy(urad_3,"?");
	}


	bcm2835_gpio_clr(LED);			//COMIENZA LA MEDIDA

	bcm2835_close();

	sprintf(msg,"mkdir %s/%.4d",base,yearg);
	system(msg);
	sprintf(msg,"mkdir %s/%.4d/%.2d",base,yearg,month);
	system(msg);
	sprintf(msg,"echo \"%.2d %.2d %.2d  %.2d %.2d %.2d  %s  %s  %s\" >> %s/%.4d/%.2d/%.2d%.2d%.2d.txt",year,month,day,hours,minutes,seconds,medida1,medida2,medida3,base,yearg,month,year,month,day);
	printf("%s\n",msg);
	system(msg);
	sprintf(msg,"echo \"%.2d %.2d %.2d  %.2d %.2d %.2d  %s  %s  %s\" >> %s/%.4d/%.2d/c%.2d%.2d%.2d.txt",year,month,day,hours,minutes,seconds,urad_1,urad_2,urad_3,base,yearg,month,year,month,day);
	printf("%s\n",msg);
	system(msg);
	//MODIFICAMOS EL CRON
	sprintf(msg,"%s/muestreo.sh %d",base_pi,muestreo);
	system(msg);

	//AHORA VEMOS SI HAY QUE CREAR UN NUEVO FICHERO DE CABECERA
	//LLAMAMOS A UN SCRIPT PARA QUE LO HAGA YA QUE ESTE TAMBIEN HABRIA QUE CREAR EL FICHERO DE CABECERA AL COMIENZO DEL DIA
	//COMPARAMOS EL FICHERO DE CABECERA ANTERIOR Y EL ACTUAL, SI ALGO HA CAMBIADO SE ACTUALIZA Y SE LLAMA AL SCRIPT, SI NO, NO SE HACE NADA

	if (strcmp(name,nameb)!=0)
	{
		printf("nombres distintos\n");
		flag_cabecera=1;
	}
	if (strcmp(nserie,nserieb)!=0)
	{
		printf("numero de series distintos\n");
		flag_cabecera=1;
	}
	if (strcmp(chan1,chan1b)!=0)
	{
		printf("canal 1 distinto\n");
		flag_cabecera=1;
	}
	if (strcmp(chan2,chan2b)!=0)
	{
		printf("canal 2 distinto\n");
		flag_cabecera=1;
	}
	if (strcmp(chan3,chan3b)!=0)
	{
		printf("canal 3 distinto\n");
		flag_cabecera=1;
	}
	if (strcmp(muest,muestb)!=0)
	{
		printf("muestreo distinto\n");
		flag_cabecera=1;
	}
	if (strcmp(gain,gainb)!=0)
	{
		printf("ganancia distinta\n");
		flag_cabecera=1;
	}
	if (strcmp(filter,filterb)!=0)
	{
		printf("filtro distinto\n");
		flag_cabecera=1;
	}
	if (strcmp(lat,latb)!=0)
	{
		printf("latitud distinta\n");
		flag_cabecera=1;
	}
	if (strcmp(lon,lonb)!=0)
	{
		printf("longitud distinta\n");
		flag_cabecera=1;
	}
	if (strcmp(alt,altb)!=0)
	{
		printf("altitud distinta\n");
		flag_cabecera=1;
	}
	if (strcmp(server,serverb)!=0)
	{
		printf("servidor distinto\n");
		flag_cabecera=1;
	}
	if (strcmp(port,portb)!=0)
	{
		printf("puerto distinto\n");
		flag_cabecera=1;
	}
	if (strcmp(folder,folderb)!=0)
	{
		printf("carpeta distinta\n");
		flag_cabecera=1;
	}

	if (flag_cabecera == 1)
	{
		printf("SE EJECUTA EL SCRIPT CABECERA\n");
		system("/home/pi/soft_inclinometro/cabecera.sh");
		system("cp /home/pi/soft_inclinometro/configura.txt /home/pi/soft_inclinometro/configurab.txt");
	}


	return 0;
}

