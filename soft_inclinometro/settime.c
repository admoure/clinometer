
//ESTE PROGRAMA SINCRONIZA EL RTC CON LA HORA DEL SISTEMA
//PARA USAR CON UN CRON Y QUE LO HAGA CADA CIERTO TIEMPO

#include <bcm2835.h>
#include <stdio.h>


int bcd_to_int (int bcd);		//transforma el formato del RTC a binario
int int_to_bcd (int in);		//transforma el formato binario a RTC

int main(void)
{
	char hor[5];
	char min[5];
	char seg[5];

	char anio[5];
	char dia[5];
	char mes[5];

	int hours,minutes,seconds,year,day,month;
	int aux;

	int fd;
	unsigned char lectura[10];
	unsigned char regad;

	FILE *pipe;		//sirve para leer salidas de comando del bash (como system)
	if (!bcm2835_init())
		return 1;
	bcm2835_i2c_begin();
	bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626);
	bcm2835_i2c_setSlaveAddress(0x50);

	//cogemos los datos de la hora del sistema
	pipe = popen("date +%H -u","r");			//devuelve la hora
	fgets(hor,3,pipe);
	pclose(pipe);
	pipe = popen("date +%M -u","r");			//devuelve los minutos
	fgets(min,3,pipe);
	pclose(pipe);
	pipe = popen("date +%S -u","r");			//devuelve los segundos
	fgets(seg,3,pipe);
	pclose(pipe);
	pipe = popen("date +%y -u","r");			//devuelve el año
	fgets(anio,3,pipe);
	pclose(pipe);
	pipe = popen("date +%m -u","r");			//devuelve el mes
	fgets(mes,3,pipe);
	pclose(pipe);
	pipe = popen("date +%d -u","r");			//devuelve el dia
	fgets(dia,3,pipe);
	pclose(pipe);
	//se convierten a entero
	hours = atoi(hor);
	minutes = atoi(min);
	seconds = atoi(seg);
	month = atoi(mes);
	day = atoi(dia);
	year = atoi(anio);
	//antes de programar el reloj hay que programar el registro del dia/año
	//y colocar los bits adecuados del año para que cuente bien los 
	//bisiestos y todo eso
	aux = year % 4;
	aux = aux << 6;
	day = int_to_bcd(day);
	//printf("auxiliar =  %d day = %x  aux + day = %x", aux,day,aux|day);

	//PROGRAMAMOS EL RELOJ
	lectura[0] = 0x02;			//empezamos en los segundos
	lectura[1] = int_to_bcd(seconds);	//segundos
	lectura[2] = int_to_bcd(minutes);	//minutos
	lectura[3] = int_to_bcd(hours);		//horas
	lectura[4] = aux|day;			//diy anio
	lectura[5] = int_to_bcd(month);
	bcm2835_i2c_write(lectura,6);
	lectura[0] = 0x10;
	lectura[1] = int_to_bcd(year);
	bcm2835_i2c_write(lectura,2);
/*	wiringPiI2CWriteReg8(fd,0x02,int_to_bcd(seconds));
	wiringPiI2CWriteReg8(fd,0x03,int_to_bcd(minutes));
	wiringPiI2CWriteReg8(fd,0x04,int_to_bcd(hours));
	wiringPiI2CWriteReg8(fd,0x05,aux|day);
	wiringPiI2CWriteReg8(fd,0x06,int_to_bcd(month));
	wiringPiI2CWriteReg8(fd,0x10,int_to_bcd(year));
*/
	//leemos para comprobar
	regad = 0x02;
	bcm2835_i2c_read_register_rs(&regad,lectura,3);
	seconds = lectura[0];
	minutes = lectura[1];
	hours = lectura[2];
	regad = 0x10;
	bcm2835_i2c_read_register_rs(&regad,lectura,1);
	year = lectura[0];
        //seconds = wiringPiI2CReadReg8(fd,0x02);
        //minutes = wiringPiI2CReadReg8(fd,0x03);
        //hours = wiringPiI2CReadReg8(fd,0x04);
	printf("Reloj sincronizado con la hora del sistema\n ");
        system("date -u\n"); 
	printf("Hora del RTC %.2x:%.2x:%.2x     anio = %.2x\n",hours,minutes,seconds,year);
	bcm2835_i2c_end();
	bcm2835_close();


	return 0;

}

int bcd_to_int (int bcd)
{
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

int int_to_bcd (int in)
{
	return ((in/10) << 4) + (in % 10);
}


