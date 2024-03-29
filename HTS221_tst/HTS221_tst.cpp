/** 
 *  example program for the use of the hts221  class 
 *  for more info see the README.MD of the repository 
 *  https://github.com/wimbeaumont/peripheral_dev_tst.git
 *  copied from the mbed online compiler envionment 
 *  V 0.1  : tested on the KL25z 
 *  V 0.21 : heater added
 *  V 0.30 : removed tsi sensor , version for git hub and mbed-cli
 *  V 0.40 : version for Linux / Raspberry Pi 
 *  V 0.41 : version for Linux / Raspberry Pi  debugging
 *  V 0.50 : version Raspberry Pi  pico only
 *  V 0.60 : version  integrated , comment out heater  
 * (C) Wim Beaumont Universiteit Antwerpen 2017 2019 2022
 *
 * License see
 * https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
 */ 

#define HTS221EXAMPLEVER "0.60"

 


#include "dev_interface_def.h"
#include "hts221.h"

// OS / platform  specific  configs 
#ifdef __PICO__ 
#include <stdio.h>
#include "pico/stdlib.h"
//#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "PicoI2CInterface.h"
PicoI2CInterface mbedi2c;
PicoI2CInterface*  mbedi2cp = &mbedi2c;
#elif  defined  __MBED__ 
#define  OS_SELECT "MBED" 

#include "mbed.h"

#if   defined (TARGET_KL25Z) || defined (TARGET_KL46Z)
  PinName const SDA = PTE0;
  PinName const SCL = PTE1;
#elif defined (TARGET_KL05Z)
  PinName const SDA = PTB4;
  PinName const SCL = PTB3;
#elif defined (TARGET_K20D50M)
  PinName const SDA = PTB1;
  PinName const SCL = PTB0;
#else
  #error TARGET NOT DEFINED
#endif

#if  MBED_MAJOR_VERSION > 5 
BufferedSerial pc(USBTX, USBRX);
#else 
Serial pc(USBTX, USBRX);
#endif Serial pc(USBTX, USBRX);

#include "I2C.h"
#include "MBEDI2CInterface.h"  
MBEDI2CInterface mbedi2c( SDA, SCL); 
MBEDI2CInterface* mbedi2cp=  &mbedi2c ;

//------------------ end MBED specific config
#elif defined __LINUX__

#define  OS_SELECT "linux_i2c" 

#include <cstdio>
#include <cstdlib>
#include "LinuxI2CInterface.h"

char *filename = (char*)"/dev/i2c-1";  //hard coded for the moment 
LinuxI2CInterface  mbedi2c(filename);
LinuxI2CInterface* mbedi2cp= &mbedi2c;

//------------------ end Linux I2C specific config
#else 
#define  OS_SELECT "linux_dummy" 

#include <cstdio>
#include <cstdlib>
#include "DummyI2CInterface.h"
DummyI2CInterface  mbedi2c;
DummyI2CInterface* mbedi2cp= &mbedi2c;

#endif  // __MBED__ 
//------------------ end Linux dummy specific config
#ifndef OS_SELECT 
#define OS_SELECT "linux_dummy" 
#endif
// --- end platform specific configs 



I2CInterface* i2cdev= mbedi2cp;



void print_buf_hex( char *data, int length){
    int nr;
    char *ptr=data;
    for ( int lc=0; lc < length ; lc++){
        nr= (int) *(ptr++);
        printf( "%02x ",nr);
    }
    printf("\n\r");
}        


int main(void) {
#ifdef __PICO__    
       stdio_init_all();// pico init 
#endif 

   
   
    float H0_rh, H1_rh;//< Humidity for calibration 
    int16_t H0_T0_out, H1_T0_out;//<Output Humidity value for calibration
    int16_t H_T_OUT;
   
   // get the version of getVersion 
   char dummystr[50];
   getVersion gv;
  
   printf("HTS221   example program version %s, compile date %s time %s\n\r",HTS221EXAMPLEVER,__DATE__,__TIME__);
   
   printf("getVersion :%s ",gv.getversioninfo());
   gv.dev_interface_def_version(dummystr);
   printf(" def type version  :%s\n\r ",dummystr);
   HTS221 shs ( i2cdev,true,true);
   printf ( "HTS221 lib version :%s\n\r ",shs.getversioninfo());

    shs.getHCalValues(  H0_rh, H1_rh, H0_T0_out,H1_T0_out);
	printf("  H0_rh %f, H1_rh %f , H0_T0_out %d ,H1_T0_out %d  \n\r", H0_rh, H1_rh, H0_T0_out,H1_T0_out);
	//int status=shs.readAllReg(  );    if ( status != 0) printf("failed to read all reg \n\r");
    float hum, Temp;
    int lc=1;
    int id;
    while(1) {
        
         //if (lc %600 ) {  
            id=(int) shs.ReadID();
            printf("lc:%04d ChipID=%02x ",lc,id );  
            
            shs.GetHumidity(&hum);
            shs.GetTemperature(&Temp);
            printf("Temperature %f,Humidity  %f   ",Temp, hum);
            shs.GetRawHumidity(&H_T_OUT);
            hum=H0_rh+ ( H1_rh -H0_rh) * (float) ( H_T_OUT - H0_T0_out) / float(H1_T0_out-H0_T0_out);
            printf("Humidity %f ,RawData %d ",hum,H_T_OUT);
            printf("\n\r");
            i2cdev->wait_for_ms(400); 
/*        }
        else {
			printf("  H0_rh %f, H1_rh %f , H0_T0_out %x ,H1_T0_out %xd  \n\r", H0_rh, H1_rh, H0_T0_out,H1_T0_out);
            printf(" on heater for 10 s lc:%d \n\r",lc);
            shs.Heater_On();
            i2cdev->wait_for_ms(5000);
            i2cdev->wait_for_ms(5000);
            shs.Heater_Off();
            i2cdev->wait_for_ms(1000);

        }
        */
		lc++;
    }
    
   
  
    return 1;
}   
