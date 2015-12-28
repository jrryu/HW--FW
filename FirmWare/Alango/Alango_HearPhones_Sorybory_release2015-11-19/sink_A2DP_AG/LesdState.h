/*
    file "LesdState.h"
*/

#ifndef _LED_STATES_H
#define _LED_STATES_H

#include "sink_states.h"
/*#include "utils.h"*/
typedef enum 
{
/*0*/enOffLED,
/*1*/enPAIRING,
/*2*/enSTANDBY,
/*3*/enSTANDBYandHearingOn,/*STANDBY with Hearing functionality*/
/*4*/enSTANDBYandPhoneConnected,/*STANDBY with Phone connected*/
/*5*/enSTANDBYandPhoneAndHearing,/*STANDBY with Phone and Hearing*/
/*6*/enSTANDBYandAssistiveDevice,/*STANDBY with Assistive Device (HearPhones, Remote Microphone, Sound Transceiver)*/
/*7*/enPHONE_ACTIVE,
/*8*/enASSISTIVE_DEVICE_ACTIVE,
/*9*/enINTERCOM_Out_Call_Waiting,/*INTERCOM Outgoing CallWaiting for answer*/
/*10*/enINTERCOMIncomingCall,
/*11*/enLOW_BATTERY,/*LOW BATTERY (less than 20%)*/
/*12*/enBATTERY_CHARGING
}stateShowLED;

typedef enum
{
        /*! No AGHF connected */	
    aghfDisconnected,
        /*! try connect to Assistive Device */	
    aghfConnecting,
        /*! connected to Assistive Device */	
    aghfConnected,
        /*! Sound connected to Assistive Device */	
    aghfConnectedSound

} aghpState;


void InitLED(void);
void SetStateLED(stateShowLED newLedState);

void ShowLedState(aghpState pNewMainState);

void ShowLedStateAGHF(aghpState pNewAGHFState);

#endif

