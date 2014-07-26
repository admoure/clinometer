//LIBRERIA DEL CONVERSOR AD7706 DE ANALOG DEVICES
#ifndef AD7706__H
#define AD7706__H

//////COMUNICATION REGISTER////////

//REGISTER SELECTION

#define  COMMUNICATION_REGISTER  0x00
#define  SETUP_REGISTER          0x10
#define  CLOCK_REGISTER          0x20
#define  DATA_REGISTER           0x30
#define  TEST_REGISTER           0x40
#define  OFFSET_REGISTER         0x60
#define  GAIN_REGISTER           0x70

//CHANNEL SELECTION

#define  CHANNEL1                0x00
#define  CHANNEL2                0x01
#define  CHANNEL3                0x03

//LECTURA/ESCRITURA

#define  _WRITE                  0x00
#define  _READ                   0x08

//STANDBY

#define  STBY_ON                 0x04
#define  STBY_OFF                0x00

////////SETUP REGISTER/////////////////

//OPERATING MODE

#define  NORMAL_MODE             0x00
#define  SELF_CALIBRATION        0x40
#define  ZERO_SCALE_SYSTEM_CAL   0x80
#define  FULL_SCALE_SYSTEM_CAL   0xC0

//GAIN SELECTION

#define  GAIN_X1                 0x00
#define  GAIN_X2                 0x08
#define  GAIN_X4                 0x10
#define  GAIN_X8                 0x18
#define  GAIN_X16                0x20
#define  GAIN_X32                0x28
#define  GAIN_X64                0x30
#define  GAIN_X128               0x38

//BIPOLAR/UNIPOLAR OPERATION

#define  BIPOLAR_OP              0x00
#define  UNIPOLAR_OP             0x04

//BUFFER CONTROL

#define  BUFFER_ON               0x02
#define  BUFFER_OFF              0x00              //se reduce la corriente

//FILTER SYNCHRONIZATION

#define  FSYNC_ON                0x01              //el filtro esta en reset
#define  FSYNC_OFF               0x00

//////////CLOCK REGISTER/////////////////////

//MASTER CLOCK DISABLE BIT

#define  CLKDIS_ON               0x10
#define  CLKDIS_OFF              0x00           //usar este con cuarzo

//CLOCK DIVIDER BIT

#define  CLKDIV_ON               0x08           //divide por dos la frecuencia
#define  CLKDIV_OFF              0x00           //USAR ESTE CON RELOJ DE 2.4576MHz

//CLOCK BIT

#define  CLK_ON                  0x04           //USAR ESTE CON 2.4576
#define  CLK_OFF                 0x00

//FILTER SELECTION BITS

#define  HZ50                    0x00
#define  HZ60                    0x01
#define  HZ250                   0x02
#define  HZ500                   0x03

//void  AD_INI(void);
//void  AD_CONFIG(char __CHAN,char __REG,char __WR,char __STBY,char __CLKDIS,char __CLKDIV,char __CLK,char __RATE );

int READ_AD_DATA(unsigned char __CHAN);

#endif



