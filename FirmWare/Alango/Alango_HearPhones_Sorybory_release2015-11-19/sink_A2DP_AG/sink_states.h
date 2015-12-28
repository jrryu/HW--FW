/****************************************************************************
Copyright (C) Cambridge Silicon Radio Ltd. 2004-2014

FILE NAME
    sink_states
    
DESCRIPTION
    The device states
*/

#ifndef _SINK_STATES_H
#define _SINK_STATES_H

/*!
@file   sink_states.h
@brief  Header file for the device application states

    This defines the device states used in the sink application

*/

/*!
	@brief The supported device states
*/
typedef enum
{
        /*! The device is logically off but physically on - limbo */	
    deviceLimbo,
        /*! The device is connectable - page scanning */
    deviceConnectable,
        /*! The device is connectable and discoverable - page and inquiry scanning*/
    deviceConnDiscoverable,
        /*! The device is connected to an AG*/
    deviceConnected,
        /*! The connected AG has an outgoing call in progress*/
    deviceOutgoingCallEstablish,
        /*! The connected AG has an incoming call in progress*/
    deviceIncomingCallEstablish,
        /*! The connected AG has an active call in progress and the audio is in the device */
    deviceActiveCallSCO ,
        /*! The device is in test mode*/
    deviceTestMode ,
        /*! The connected AG has an active call and a second incoming call*/
    deviceThreeWayCallWaiting,
        /*! The connected AG has an active call and a second call on hold*/
    deviceThreeWayCallOnHold,
        /*! The connected AG has more than one active call*/
    deviceThreeWayMulticall,
        /*! The connected AG has an incoming call on hold*/
    deviceIncomingCallOnHold , 
        /*! The connected AG has an active call and the audio is in the handset*/
    deviceActiveCallNoSCO ,
        /*! The device is streaming A2DP audio */
    deviceA2DPStreaming ,
        /* low battery state, won't actually change to this state but will be used for independant 
           low battery led warning */
    deviceLowBattery
	
} sinkState;

/*!
	@brief The maximum number of device states -keep this value up to date if new stated are added
*/
#define SINK_NUM_STATES (deviceLowBattery + 1) 





#endif

