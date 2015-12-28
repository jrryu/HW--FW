#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sink.h>
#include <source.h>
#include <boot.h>
#include <stream.h>
#include <panic.h>
#include <i2c.h>
#include <print.h>
#include <led.h>

#include <ps.h>
#include <bdaddr.h>

#include "sink_configmanager.h"
#include "sink_private.h"
#include "audio_plugin_if.h"

#include "utils.h"
#include "sink_statemanager.h"
#include "LesdState.h"
#include "sink_tones.h"

#define DEBUG_MYUTILS

#ifdef DEBUG_MYUTILS
 #define MYUTILS_DEBUG(x) DEBUG(x)
#else
 #define MYUTILS_DEBUG(x) 
#endif



bdaddr newSalt;/*= PanicUnlessMalloc(req->length);*/
uint8 SerachSALT=0;
uint8 AGHFPOutCall=0;
uint8 CntrSendRing=0;
#define TIME_PLAY_TONE_INTERCOM_RING 4

#include <message.h>
#include <pio.h>



/***************************************************************/

void InitAllData(void)
{
    MYUTILS_DEBUG(("InitAllData\n\r"));        
    newSalt.nap=0;
    newSalt.uap=0;
    newSalt.lap=0;

    /*PsRetrieve(CONFIG_IR_REMOTE_CONTROL, &theSink.AGvolume, sizeof(theSink.AGvolume));*/
    /*MYUTILS_DEBUG(("theSink.AGvolume=%d\n\r",theSink.AGvolume));*/

/*	InitLED();*/
if (0)
{    
/* Led initialising */
LedConfigure(LED_0, LED_DUTY_CYCLE, 0x7FF);
/* Set the PWM period */
LedConfigure(LED_0, LED_PERIOD, 0x0);
/* Pulse the LEDs */
LedConfigure(LED_0, LED_FLASH_ENABLE, 1);
LedConfigure(LED_0, LED_FLASH_RATE, 0x7); /* Pulse the LED_0 slowly */
LedConfigure(LED_1, LED_DUTY_CYCLE, 0x7FF);
/* Set the PWM period */
LedConfigure(LED_1, LED_PERIOD, 0x0);
/* Pulse the LEDs */
LedConfigure(LED_1, LED_FLASH_ENABLE, 1);
LedConfigure(LED_1, LED_FLASH_RATE, 0x7); /* Pulse the LED_0 slowly */
LedConfigure(LED_2, LED_DUTY_CYCLE, 0x7FF);
/* Set the PWM period */
LedConfigure(LED_2, LED_PERIOD, 0x0);
/* Pulse the LEDs */
LedConfigure(LED_2, LED_FLASH_ENABLE, 1);
LedConfigure(LED_2, LED_FLASH_RATE, 0x7); /* Pulse the LED_0 slowly */
 /* Turning on the compatible leds with profile number. */

/*LedConfigure( LED_0, LED_ENABLE, (1));*/
/*LedConfigure( LED_1, LED_ENABLE, (1));*/
LedConfigure( LED_2, LED_ENABLE, (1));
}

}
#ifdef ENABLE_AG
 void StartInquireSALT(void);
 void StartInquireSALT(void)
 {        
    MYUTILS_DEBUG(("Start ConnectionInquire\n\r"));
/* Inquiry defines */
#define INQUIRY_LAP                 0x9e8b33
#define INQUIRY_MAX_RESPONSES       1
#define INQUIRY_TIMEOUT             4           /* timeout * 1.28 seconds */ 
#define INQUIRY_SCAN_BUFFER_SIZE    10
#define COD_MAJOR_AV                                    0x000400                
    /*ConnectionSmDeleteAllAuthDevices(0);????*/
    theSink.HearPhones_conf->Ag.ag_bd_addr.nap = 0;
    theSink.HearPhones_conf->Ag.ag_bd_addr.uap = 0;
    theSink.HearPhones_conf->Ag.ag_bd_addr.lap = 0;   
    ConnectionInquire(&theSink.task, INQUIRY_LAP, INQUIRY_MAX_RESPONSES, INQUIRY_TIMEOUT, (uint32)COD_MAJOR_AV);                     
 }
#endif 
void RxDataUART (void)
{
    uint8 *rxPtr;
    int size,i;

    size = SourceSize(StreamUartSource());
    if(size)
    {
        rxPtr = (uint8*)SourceMap(StreamUartSource());				
		for (i=0;i<size;i++)
		{
            /*MYUTILS_DEBUG(("Rx Pack = %c, Host.lenBufConsole = %d\n\r",*rxPtr,Host.lenBufConsole));*/
            MYUTILS_DEBUG(("Rx char = %c\n\r",*rxPtr));
            switch(*rxPtr)
            {
                /*case '0':{SetStateLED(enPAIRING);};break;
                case '1':{SetStateLED(enSTANDBY);};break;
                case '2':{SetStateLED(enSTANDBYandHearingOn);};break;
                case '3':{SetStateLED(enSTANDBYandPhoneConnected);};break;
                case '4':{SetStateLED(enSTANDBYandPhoneAndHearing);};break;
                case '5':{SetStateLED(enSTANDBYandAssistiveDevice);};break;
                case '6':{SetStateLED(enPHONE_ACTIVE);};break;
                case '7':{SetStateLED(enASSISTIVE_DEVICE_ACTIVE);};break;
                case '8':{SetStateLED(enINTERCOM_Out_Call_Waiting);};break;
                case '9':{SetStateLED(enINTERCOMIncomingCall);};break;
                case 'b':{SetStateLED(enLOW_BATTERY);};break;
                case 'B':{SetStateLED(enBATTERY_CHARGING);};break;*/
                                                 
                case 'I':
                {
    };break;

               case 'D':
               {
                   MYUTILS_DEBUG(("ResetPairedDeviceList\n")) ;
                    MessageSendLater(&theSink.task,  EventUsrResetPairedDeviceList, 0, D_SEC(1));                            
               };break;    
               case 'P':
               {    MYUTILS_DEBUG(("Start Pairing\n")) ;
                    MessageSendLater(&theSink.task,  EventUsrEnterPairing, 0, D_SEC(1));                            
                
               };break;                    
               case 'p':
               { 
                    MYUTILS_DEBUG(("Stop Pairing\n")) ;
                    MessageCancelAll ( &theSink.task , EventSysPairingFail ) ;
                    MessageSendLater ( &theSink.task , EventSysPairingFail , 0 , D_SEC(1) ) ;                   
               }
               case 'n':
               { 
                    /*PsRetrieve(CONFIG_FM_CONFIG, &theSink.ag_bd_addr, sizeof(bdaddr));*/
                    /*ConnectionReadRemoteName(&theSink.task, &theSink.ag_bd_addr);*/
               }                
               case 'x':
               {
                    {/*uint16 ret_len = 0;    */
                        /*ret_len = PsRetrieve(CONFIG_FM_CONFIG, &theSink.ag_bd_addr, sizeof(bdaddr));*/
                        /*MYUTILS_DEBUG(("MYUTILS: Read BT Adr[%u]len[0x%x]\n",CONFIG_FM_CONFIG, ret_len)) ;*/
                    }            
                    /*MYUTILS_DEBUG(("** Deleta to Addr %04x,%02x,%06lx \n", theSink.ag_bd_addr.nap,
                                                                    theSink.ag_bd_addr.uap,
                                                                    theSink.ag_bd_addr.lap)) ;*/
                   /*ConnectionSmDeleteAuthDevice(&theSink.ag_bd_addr);*/
               };break;                    
            }
        }            
    }
    SourceDrop(StreamUartSource(), size);    
}
/*----------------------------------------------------------------------------*/
bool IsSameBTAdrSALT(bdaddr * BTAdrCompare);
bool IsSameBTAdrSALT(bdaddr * BTAdrCompare)
{bdaddr PskeyAdrSalt;
uint16 ret_len = 0;    
    ret_len = PsRetrieve(CONFIG_FM_CONFIG, &PskeyAdrSalt, sizeof(PskeyAdrSalt));
    if (ret_len) {return (BdaddrIsSame(BTAdrCompare, &PskeyAdrSalt));};
    return FALSE;
    
}
/*----------------------------------------------------------------------------*/
bool BTAdrSALTStored(void);
bool BTAdrSALTStored(void)
{bdaddr PskeyAdrSalt;
uint16 ret_len = 0;    
    ret_len = PsRetrieve(CONFIG_FM_CONFIG, &PskeyAdrSalt, sizeof(PskeyAdrSalt));
    if (ret_len) {return (TRUE);};
    return FALSE;    
}



void ExpiredTimer(void)
{
      
}





