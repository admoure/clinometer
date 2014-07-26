#include <bcm2835.h>
#include "ad7706.h"
#include <stdio.h>
/*void  AD_INI(int8 _CHAN)
{

   output_high(CS_AD);           //DESHABILITAMOS CONVERSOR

   //RESET DEL CONVERSOR

   output_low(RESET_AD);
   delay_ms(5);
   output_high(RESET_AD);

   output_low(CS_AD);             //HABILITAMOS CONVERSOR

   //INICIALIZACION DEL CONVERSOR, CONFIGURAMOS EL CANAL 1 A MUESTRAS POR SEGUNDO

   AD_CONFIG(_CHAN,CLOCK_REGISTER,_WRITE,STBY_OFF,CLKDIS_OFF,CLKDIV_OFF,CLK_ON,HZ50);
   AD_CONFIG(_CHAN,SETUP_REGISTER,_WRITE,STBY_OFF,SELF_CALIBRATION,GAIN_X1,UNIPOLAR_OP,BUFFER_OFF,FSYNC_OFF);

   output_high(CS_AD);           //DESHABILITAMOS CONVERSOR


}
*/
//ACCESO AL CLOCK REGISTER


/*int READ_AD_DATA(unsigned char __CHAN)
{
	unsigned char datoh,datol,temp,rerun;
	unsigned int dato,sumatorio;

  	 //PRIMERO HACEMOS UNA LECTURA DEL PROPIO REGISTRO DE STADO PARA LEER EL DRDY
	sumatorio = 0;
	for (rerun=0;rerun<32;rerun++)
	{
		do{
			temp = (COMMUNICATION_REGISTER|_READ|__CHAN);
			bcm2835_spi_transfer(temp);
			temp = bcm2835_spi_transfer(0);
			printf("%x\n",temp);

		}while((temp&0x80)!=0);		//cuando drdy este a 0 sale

		temp = (DATA_REGISTER|_READ|__CHAN);
		bcm2835_spi_transfer(temp);
		datoh = bcm2835_spi_transfer(0);
		datol = bcm2835_spi_transfer(0);
		dato = (unsigned int)datoh * 256 + (unsigned int)datol;
		sumatorio = sumatorio + (unsigned int)dato;
	}

	dato = sumatorio / 32;
	return dato;
}*/



