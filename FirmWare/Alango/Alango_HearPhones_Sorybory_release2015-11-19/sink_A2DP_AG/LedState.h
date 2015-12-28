/*
    file "LedState.h"
*/

#ifndef _LED_STATES_H
#define _LED_STATES_H

#include "sink_states.h"
/*#include "utils.h"*/
typedef enum 
{
    enOffLED,
    enPAIRING,
    enSTANDBY,
    enSTANDBYandHearingOn,/*STANDBY with Hearing functionality*/
    enSTANDBYandPhoneConnected,/*STANDBY with Phone connected*/
    enSTANDBYandPhoneAndHearing,/*STANDBY with Phone and Hearing*/
    enSTANDBYandAssistiveDevice,/*STANDBY with Assistive Device (HearPhones, Remote Microphone, Sound Transceiver)*/
    enPHONE_ACTIVE,
    enASSISTIVE_DEVICE_ACTIVE,
    enINTERCOM_Out_Call_Waiting,/*INTERCOM Outgoing CallWaiting for answer*/
    enINTERCOMIncomingCall,
    enLOW_BATTERY,/*LOW BATTERY (less than 20%)*/
    enBATTERY_CHARGING
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

