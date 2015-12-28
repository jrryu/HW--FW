#include "LesdState.h"
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
#include "sink_private.h"
#include "sink_statemanager.h"

uint8 onLED=0;
#define ON_LED_GREEN    0x01
#define ON_LED_BLUE     0x02
#define ON_LED_RED     0x04

stateShowLED stShowLED;

void led_controller( Task t, MessageId id, Message payload );

static TaskData led_controller_task = { led_controller };
/***************************************************************/
#define DELAY1   500 		/* ms */
#define LED_BLUE    LED_1 
#define LED_RED     LED_2 
#define LED_GREEN   LED_0

#define TIME_ON_LED 20
#define TIME_OFF_LED_FREQUENTLY 1000
#define TIME_OFF_LED_RARELY     3000
#define TIME_OFF_LED_VERY_RARELY     6000
/*************************************************************
FREQUENTLY - 1 time in 1 second, the pulse duration – 20 ms
RARELY – 1 time in 3 seconds, the pulse duration – 20 ms
VERY RARELY - 1 time in 6 seconds, the pulse duration – 20 ms

PAIRING	                             BLUE and RED  FREQUENTLY 
STANDBY	                             PURPLE RARELY
STANDBY with Hearing functionality	 BLUE VERY RARELY
STANDBY with Phone connected	     YELLOW VERY RARELY 
STANDBY with Phone and Hearing	     BLUE and YELLOW - VERY RARELY 
STANDBY with Assistive Device 	     GREEN - VERY RARELY
        (HearPhones, Remote Microphone, Sound Transceiver)
PHONE ACTIVE	                     YELLOW RARELY
ASSISTIVE DEVICE ACTIVE	             GREEN RARELY
INTERCOM Outgoing Call
Waiting for answer	                  RED and GREEN - FREQUENTLY
INTERCOM Incoming Call	              RED and GREEN - FREQUENTLY
LOW BATTERY (less than 20%)	          RED alternately with any other light (depending on the current mode). In STANDBY mode – RED - VERY RARELY
BATTERY CHARGING	                  RED SOLID  
       When charging is complete, the LED indicator will turn off

*************************************************************/

void InitLED(void)
{
    stShowLED=enOffLED;/*enPHONE_ACTIVE;enASSISTIVE_DEVICE_ACTIVE;enINTERCOMIncomingCall;enLOW_BATTERY;enBATTERY_CHARGING;enSTANDBYandPhoneAndHearing;enSTANDBYandPhoneConnected;enSTANDBYandHearingOn;enSTANDBY;enPAIRING;*/
    MessageSend( &led_controller_task, 0 , 0 );  
}


void SetStateLED(stateShowLED newLedState)
{
    onLED=0;
    stShowLED=newLedState;
}
        
void led_controller( Task t, MessageId id, Message payload )
{int NextDelay=10;

    LedConfigure(LED_GREEN, LED_ENABLE, 0);      
    LedConfigure(LED_RED,   LED_ENABLE, 0);       
    LedConfigure(LED_BLUE,  LED_ENABLE, 0);      
    switch(stShowLED)    
    {
        case enOffLED:{
            LedConfigure(LED_GREEN, LED_ENABLE, 0);      
            LedConfigure(LED_RED,   LED_ENABLE, 0);       
            LedConfigure(LED_BLUE,  LED_ENABLE, 0);      
            onLED=0;
            NextDelay=TIME_ON_LED;
        };break;
        case enPAIRING:{/*BLUE and RED  FREQUENTLY */
            if (onLED==0)
            {
                onLED=1;                
                LedConfigure(LED_BLUE, LED_ENABLE, 1);      
                LedConfigure(LED_RED, LED_ENABLE, 0);      
                NextDelay=TIME_ON_LED;
            }
            else
            if (onLED==1)
            {
                onLED=2;
                LedConfigure(LED_BLUE, LED_ENABLE, 0);      
                LedConfigure(LED_RED, LED_ENABLE, 0);      
                NextDelay=TIME_OFF_LED_FREQUENTLY;
            }
            else
            if (onLED==2)
            {
                onLED=3;
                LedConfigure(LED_BLUE, LED_ENABLE, 0);      
                LedConfigure(LED_RED, LED_ENABLE, 1);      
                NextDelay=TIME_ON_LED;
            }
            else
            if (onLED==3)
            {
                onLED=0;
                LedConfigure(LED_BLUE, LED_ENABLE, 0);      
                LedConfigure(LED_RED, LED_ENABLE, 0);      
                NextDelay=TIME_OFF_LED_FREQUENTLY;
            }                      
         };break;
        case enSTANDBY:{/*PURPLE RARELY*/
            if (onLED==0)
            {
                onLED=1;                
                LedConfigure(LED_BLUE, LED_ENABLE, 1);      
                LedConfigure(LED_RED, LED_ENABLE, 1);      
                NextDelay=TIME_ON_LED;
            }
            else
            if (onLED==1)
            {
                onLED=0;
                LedConfigure(LED_BLUE, LED_ENABLE, 0);      
                LedConfigure(LED_RED, LED_ENABLE, 0);      
                NextDelay=TIME_OFF_LED_RARELY;
            }
         };break;
        case enSTANDBYandHearingOn:{/*BLUE VERY RARELY*/
            if (onLED==0)
            {
                onLED=1;                
                LedConfigure(LED_BLUE, LED_ENABLE, 1);      
                NextDelay=TIME_ON_LED;
            }
            else
            if (onLED==1)
            {
                onLED=0;
                LedConfigure(LED_BLUE, LED_ENABLE, 0);      
                NextDelay=TIME_OFF_LED_VERY_RARELY;
            }         
          };break;
        case enSTANDBYandPhoneConnected:{/*YELLOW VERY RARELY */
            if (onLED==0)
            {
                onLED=1;                
                LedConfigure(LED_GREEN, LED_ENABLE, 1);      
                LedConfigure(LED_RED, LED_ENABLE, 1);      
                NextDelay=TIME_ON_LED;
            }
            else
            if (onLED==1)
            {
                onLED=0;
                LedConfigure(LED_GREEN, LED_ENABLE, 0);      
                LedConfigure(LED_RED, LED_ENABLE, 0);      
                NextDelay=TIME_OFF_LED_VERY_RARELY;
            }
            
          };break;
        case enSTANDBYandPhoneAndHearing:{/*BLUE and YELLOW - VERY RARELY */
            if (onLED==0)
            {
                onLED=1;                
                LedConfigure(LED_BLUE,  LED_ENABLE, 1);      
                LedConfigure(LED_RED,   LED_ENABLE, 0);      
                LedConfigure(LED_GREEN, LED_ENABLE, 0);      
                NextDelay=TIME_ON_LED;
            }
            else
            if (onLED==1)
            {
                onLED=2;
                LedConfigure(LED_BLUE,  LED_ENABLE, 0);      
                LedConfigure(LED_RED,   LED_ENABLE, 0);      
                LedConfigure(LED_GREEN, LED_ENABLE, 0);      
                NextDelay=TIME_OFF_LED_VERY_RARELY;
            }
            else
            if (onLED==2)
            {
                onLED=3;
                LedConfigure(LED_BLUE,  LED_ENABLE, 0);      
                LedConfigure(LED_RED,   LED_ENABLE, 1);      
                LedConfigure(LED_GREEN, LED_ENABLE, 1);      
                NextDelay=TIME_ON_LED;
            }
            else
            if (onLED==3)
            {
                onLED=0;
                LedConfigure(LED_BLUE,  LED_ENABLE, 0);      
                LedConfigure(LED_RED,   LED_ENABLE, 0);      
                LedConfigure(LED_GREEN, LED_ENABLE, 0);      
     
                NextDelay=TIME_OFF_LED_VERY_RARELY;
            }                  
         };break;
        case enSTANDBYandAssistiveDevice:{/*GREEN - VERY RARELY*/
            if (onLED==0)
            {
                onLED=1;                
                LedConfigure(LED_GREEN, LED_ENABLE, 1);      
                NextDelay=TIME_ON_LED;
            }
            else
            if (onLED==1)
            {
                onLED=0;
                LedConfigure(LED_GREEN, LED_ENABLE, 0);      
                NextDelay=TIME_OFF_LED_VERY_RARELY;
            }
         };break;
        case enPHONE_ACTIVE:{/*YELLOW RARELY*/
            if (onLED==0)
            {
                onLED=1;                
                LedConfigure(LED_GREEN, LED_ENABLE, 1);      
                LedConfigure(LED_RED, LED_ENABLE, 1);      
                NextDelay=TIME_ON_LED;
            }
            else
            if (onLED==1)
            {
                onLED=0;
                LedConfigure(LED_GREEN, LED_ENABLE, 0);      
                LedConfigure(LED_RED, LED_ENABLE, 0);      
                NextDelay=TIME_OFF_LED_RARELY;
            }         
         };break;
        case enASSISTIVE_DEVICE_ACTIVE:{/*GREEN RARELY*/
            if (onLED==0)
            {
                onLED=1;                
                LedConfigure(LED_GREEN, LED_ENABLE, 1);      
                NextDelay=TIME_ON_LED;
            }
            else
            if (onLED==1)
            {
                onLED=0;
                LedConfigure(LED_GREEN, LED_ENABLE, 0);      
                NextDelay=TIME_OFF_LED_RARELY;
            }       
         };break;
        case enINTERCOM_Out_Call_Waiting:
         case enINTERCOMIncomingCall:{/*RED and GREEN - FREQUENTLY*/
           if (onLED==0)
            {
                onLED=1;                
                LedConfigure(LED_BLUE,  LED_ENABLE, 0);      
                LedConfigure(LED_RED,   LED_ENABLE, 1);      
                LedConfigure(LED_GREEN, LED_ENABLE, 0);      
                NextDelay=TIME_ON_LED;
            }
            else
            if (onLED==1)
            {
                onLED=2;
                LedConfigure(LED_BLUE,  LED_ENABLE, 0);      
                LedConfigure(LED_RED,   LED_ENABLE, 0);      
                LedConfigure(LED_GREEN, LED_ENABLE, 0);      
                NextDelay=TIME_OFF_LED_FREQUENTLY;
            }
            else
            if (onLED==2)
            {
                onLED=3;
                LedConfigure(LED_BLUE,  LED_ENABLE, 0);      
                LedConfigure(LED_RED,   LED_ENABLE, 0);      
                LedConfigure(LED_GREEN, LED_ENABLE, 1);      
                NextDelay=TIME_ON_LED;
            }
            else
            if (onLED==3)
            {
                onLED=0;
                LedConfigure(LED_BLUE,  LED_ENABLE, 0);      
                LedConfigure(LED_RED,   LED_ENABLE, 0);      
                LedConfigure(LED_GREEN, LED_ENABLE, 0);      
     
                NextDelay=TIME_OFF_LED_FREQUENTLY;
            }   
         };break;
        case enLOW_BATTERY:{
            if (onLED==0)
            {
                onLED=1;                
                LedConfigure(LED_RED, LED_ENABLE, 1);      
                NextDelay=TIME_ON_LED;
            }
            else
            if (onLED==1)
            {
                onLED=0;
                LedConfigure(LED_RED, LED_ENABLE, 0);      
                NextDelay=TIME_OFF_LED_VERY_RARELY;
            }       

         };break;
        case enBATTERY_CHARGING:{/*RED SOLID  
                    When charging is complete, the LED indicator will turn off*/
                LedConfigure(LED_RED,   LED_ENABLE, 1);           
                NextDelay=TIME_ON_LED;
         
         };break;
        
    }
    
    /*Green LED*/
	/*if (StateLED&ON_LED_GREEN)
    {
        StateLED&=(0xFF^ON_LED_GREEN);
        LedConfigure(LED_0, LED_ENABLE, 0);
    }
    else
    {
        StateLED|=ON_LED_GREEN;
        LedConfigure(LED_0, LED_ENABLE, 1);
    }*/
    /*Blue LED*/
	/*if (StateLED&ON_LED_BLUE)
    {
        StateLED&=(0xFF^ON_LED_BLUE);
        LedConfigure(LED_1, LED_ENABLE, 0);
    }
    else
    {
        StateLED|=ON_LED_BLUE;
        LedConfigure(LED_1, LED_ENABLE, 1);
    }*/
    /*if (onLED&ON_LED_RED)
    {
        onLED&=(0xFF^ON_LED_RED);
        LedConfigure(LED_2, LED_ENABLE, 0);
        NextDelay=1000;
    }
    else
    {
        onLED=ON_LED_RED;
        LedConfigure(LED_2, LED_ENABLE, 1);
        NextDelay=20;
    }*/
	MessageSendLater( t, 0, 0, NextDelay );
}

/*extern bool CsrExamplePluginIsHearing(void);*/
/*----------------------------------------------------------------------------*/
void ShowLedState(sinkState pNewMainState)
{
    switch(pNewMainState)
    {
        /*! The device is logically off but physically on - limbo */	
        case deviceLimbo:
        {
            SetStateLED(enOffLED);        
        };break;
        /*! The device is connectable - page scanning */
        case deviceConnectable:
        {
            if (theSink.HearPhones_conf->Hearing.ActiveHearing) {SetStateLED(enSTANDBYandHearingOn);}
                else     {SetStateLED(enSTANDBY);}
        };break;
        /*! The device is connectable and discoverable - page and inquiry scanning*/
        case deviceConnDiscoverable:
        {
            SetStateLED(enPAIRING);
        };break;
        /*! The device is connected to an AG*/
        case deviceConnected:
        {
            if (theSink.HearPhones_conf->Hearing.ActiveHearing) { SetStateLED(enSTANDBYandPhoneAndHearing);}
                else {SetStateLED(enSTANDBYandPhoneConnected);}
        };break;
        /*! The connected AG has an outgoing call in progress*/
        case deviceOutgoingCallEstablish:
        {
            SetStateLED(enPHONE_ACTIVE);
        };break;
        /*! The connected AG has an incoming call in progress*/
        case deviceIncomingCallEstablish:
        {
            SetStateLED(enPHONE_ACTIVE);
        };break;
        /*! The connected AG has an active call in progress and the audio is in the device */
        case deviceActiveCallSCO:
        {
            SetStateLED(enPHONE_ACTIVE);
        };break;
        /*! The device is in test mode*/
        case deviceTestMode:
        {
        };break;
        /*! The connected AG has an active call and a second incoming call*/
        case deviceThreeWayCallWaiting:
        {
            SetStateLED(enPHONE_ACTIVE);
        };break;
        /*! The connected AG has an active call and a second call on hold*/
        case deviceThreeWayCallOnHold:
        {
            SetStateLED(enPHONE_ACTIVE);
        };break;
        /*! The connected AG has more than one active call*/
        case deviceThreeWayMulticall:
        {
            SetStateLED(enPHONE_ACTIVE);
        };break;
        /*! The connected AG has an incoming call on hold*/
        case deviceIncomingCallOnHold:
        {
        };break; 
        /*! The connected AG has an active call and the audio is in the handset*/
        case deviceActiveCallNoSCO:
        {
            SetStateLED(enPHONE_ACTIVE);
        };break;
        /*! The device is streaming A2DP audio */
        case deviceA2DPStreaming:
        {
            SetStateLED(enPHONE_ACTIVE);
        };break;
        /* low battery state, won't actually change to this state but will be used for independant 
           low battery led warning */
        case deviceLowBattery:
        {
            SetStateLED(enLOW_BATTERY);
        };break;             
    }/* END switch(pNewMainState)*/
}
/*----------------------------------------------------------------------------*/
void ShowLedStateAGHF(aghpState pNewAGHFState)
{
    switch(pNewAGHFState)
    {
         /*! No AGHF connected */	
        case aghfDisconnected:
        {
           ShowLedState(stateManagerGetState()); 
        };break;
        /*! try connect to Assistive Device */	
        case aghfConnecting:
        {
            
        };break;
        /*! connected to Assistive Device */	
        case aghfConnected:
        {
            
        };break;
        /*! Sound connected to Assistive Device */	
        case aghfConnectedSound:
        {
            ShowLedState(enASSISTIVE_DEVICE_ACTIVE);
        };break;             
    }
    
}
/*----------------------------------------------------------------------------*/

