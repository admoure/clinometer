#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bcm2835.h>
#include "ad7706.h"

#define RESET_AD RPI_V2_GPIO_P1_07
#define LED	 RPI_V2_GPIO_P1_15

int result1,result2,result3;
char medida1[6],medida2[6],medida3[6];

unsigned char datoh,datol,temp,rerun;
unsigned int dato,sumatorio;
unsigned char caracteres[128];

char msg[128];
char base[256]={"/media/CAM/datalogger/inclinometro"};
char file_configura[256]={"/home/pi/soft_inclinometro/configura.txt"};
char file_calibra[256]={"/home/pi/soft_inclinometro/calib.txt"};
char base_pi[256]={"/home/pi/soft_inclinometro"};
FILE *fc;
FILE *fp;
FILE *fcali;

unsigned char lectura[10];
unsigned char regad;
char horas[3],minutos[3],segundos[3],dia[3],mes[3],anio[3];
int hours,minutes,seconds,year,day,month;
char chan1[4],chan2[4],chan3[4],muest[6],name[10];
char gain[4],filter[4],lat[16],lon[16],alt[16];
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
	fscanf(fc,"CHANNEL1=%s\n",chan1);
	fscanf(fc,"CHANNEL2=%s\n",chan2);
	fscanf(fc,"CHANNEL3=%s\n",chan3);
	fscanf(fc,"MUESTREO=%s\n",muest);
	fscanf(fc,"HIGHGAIN=%s\n",gain);
	fscanf(fc,"FILTER=%s\n",filter);
	fscanf(fc,"LAT=%s\n",lat);
	fscanf(fc,"LON=%s\n",lon);
	fscanf(fc,"ALT=%s\n",alt);
	fclose(fc);
	printf("NOMBRE DE LA ESTACION > %s\n",name);
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
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);	//1MHz
	//se habilita el i2c
//	bcm2835_i2c_begin();
//	bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626);
//	bcm2835_i2c_setSlaveAddress(0x50);


	bcm2835_gpio_set(LED);			//COMIENZA LA MEDIDA
	//TOMAMOS LA HORA DE LA MEDIDA//HAY QUE HACER CONTROL DE CALIDAD

//	regad = 0x02;
//	bcm2835_i2c_read_register_rs(&regad,lectura,5);
	fp = popen("date +%S","r");
	fgets(segundos,3,fp);
	pclose(fp);
	fp = popen("date +%M","r");
	fgets(minutos,3,fp);
	pclose(fp);
	fp = popen("date +%H","r");
	fgets(horas,3,fp);
	pclose(fp);
	fp = popen("date +%y","r");
	fgets(anio,3,fp);
	pclose(fp);
	fp = popen("date +%m","r");
	fgets(mes,3,fp);
	pclose(fp);
	fp = popen("date +%d","r");
	fgets(dia,3,fp);
	pclose(fp);
	seconds=atoi(segundos);
	minutes=atoi(minutos);
	hours=atoi(horas);
	month=atoi(mes);
	year=atoi(anio);
	day=atoi(dia);

	//reiniciamos conversor

	bcm2835_gpio_clr(RESET_AD);
	bcm2835_delay(100);
	bcm2835_gpio_set(RESET_AD);
	bcm2835_delay(300);
	while(1)
	{
		bcm2835_spi_transfer(CLOCK_REGISTER|CHANNEL1|_WRITE);
		bcm2835_spi_transfer(CLKDIS_OFF|CLKDIV_OFF|CLK_ON|HZ50);
		bcm2835_spi_transfer(SETUP_REGISTER|CHANNEL1|_WRITE);
		bcm2835_spi_transfer(SELF_CALIBRATION|GAIN_X1|UNIPOLAR_OP|BUFFER_OFF|FSYNC_OFF);

		bcm2835_delay(300);
		sumatorio = 0;
		for (rerun=0;rerun<32;rerun++)
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
		result1 = sumatorio/32;
		cuentas_a_voltaje_canal1 = (((float)result1 * 2.5)/65535.);
		//printf("voltaje canal1 -> %f\n",cuentas_a_voltaje_canal1);
		vec1 = ((cuentas_a_voltaje_canal1 - vb1)/va1)*1000;
		//printf("voltaje salida sensor canal1 -> %f mv\n",vec1);
/*		if(strcmp(gain,"ON")==0)
		{
			urad_canal1 = (a1h*vec1*vec1*vec1*vec1*vec1)+(b1h*vec1*vec1*vec1*vec1)+(c1h*vec1*vec1*vec1)+(d1h*vec1*vec1)+(e1h*vec1)+f1h;
		}
		else
		{
			urad_canal1 = (a1l*vec1*vec1*vec1*vec1*vec1)+(b1l*vec1*vec1*vec1*vec1)+(c1l*vec1*vec1*vec1)+(d1l*vec1*vec1)+(e1l*vec1)+f1l;
		}
		printf("RAD_CANAL_1 -> %f urad\n",urad_canal1);
// result1 = READ_AD_DATA(DATA_REGISTER|_READ|CHANNEL1);
		sprintf(medida1,"%d",result1);
		sprintf(urad_1,"%.2f",urad_canal1);*/


		bcm2835_spi_transfer(CLOCK_REGISTER|CHANNEL2|_WRITE);
		bcm2835_spi_transfer(CLKDIS_OFF|CLKDIV_OFF|CLK_ON|HZ50);
		bcm2835_spi_transfer(SETUP_REGISTER|CHANNEL2|_WRITE);
		bcm2835_spi_transfer(SELF_CALIBRATION|GAIN_X1|UNIPOLAR_OP|BUFFER_OFF|FSYNC_OFF);

		bcm2835_delay(300);
		sumatorio = 0;
		for (rerun=0;rerun<32;rerun++)
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
		result2 = sumatorio/32;
		cuentas_a_voltaje_canal2 = (((float)result2 * 2.5)/65535.);
		//printf("cuentas a voltaje canal2 -> %f\n",cuentas_a_voltaje_canal2);
		vec2 = ((cuentas_a_voltaje_canal2 - vb2)/va2)*1000;
		//printf("voltaje salida sensor canal2 -> %f mv\n",vec2);


		bcm2835_spi_transfer(CLOCK_REGISTER|CHANNEL3|_WRITE);
		bcm2835_spi_transfer(CLKDIS_OFF|CLKDIV_OFF|CLK_ON|HZ50);
		bcm2835_spi_transfer(SETUP_REGISTER|CHANNEL3|_WRITE);
		bcm2835_spi_transfer(SELF_CALIBRATION|GAIN_X1|UNIPOLAR_OP|BUFFER_OFF|FSYNC_OFF);

		bcm2835_delay(300);
		sumatorio = 0;
		for (rerun=0;rerun<32;rerun++)
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
			sumatorio = sumatorio + (unsigned int)dato;
		}
		result3 = sumatorio/32;
		cuentas_a_voltaje_canal3 = (((float)result3 * 2.5)/65535.);
//		printf("cuentas a voltaje canal3 -> %f\n",cuentas_a_voltaje_canal3);
		vec3 = ((cuentas_a_voltaje_canal3 - vb3)/va3)*1000;
//		printf("voltaje salida sensor canal3 -> %f mv\n",vec3);

		printf("CORTO CH1 A/D: %f V   CORTO CH2 A/D: %f V   CORTO CH3 A/D: %f V\t|\tSENSOR X: %f mV  SENSOR Y: %f mV  SENSOR T: %f mV \n",cuentas_a_voltaje_canal1,cuentas_a_voltaje_canal2,cuentas_a_voltaje_canal3,vec1,vec2,vec3);
	}


	bcm2835_gpio_clr(LED);			//COMIENZA LA MEDIDA

	bcm2835_close();

	return 0;
}

