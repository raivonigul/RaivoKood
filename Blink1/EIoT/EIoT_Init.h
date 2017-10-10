// EIoT_Init.h

#ifndef _EIOT_INIT_h
#define _EIOT_INIT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include "EIoT.h"



void createNewConfig(struct StoreStruct *storage, EIoTCloudRestApi eiotcloud);

#endif

