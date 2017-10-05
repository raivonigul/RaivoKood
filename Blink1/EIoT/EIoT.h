#ifndef _EIOT_h
#define _EIOT_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define INSTANCE_ID "58bdd09747976c2bde23fa00"
#ifdef DEBUG_PROG
#define DEBUG_PRINTLN(x)  Serial.println(x)
#define DEBUG_PRINT(x)    Serial.print(x)
#define REPORT_INTERVAL 1
#else
#define DEBUG_PRINTLN(x) 
#define DEBUG_PRINT(x)
#define REPORT_INTERVAL 10
#endif
#include "EIoTCloudRestApiV1.0.h"


struct StoreStruct {
	// This is for mere detection if they are your settings
	char version[4];
	// The variables of your settings
	char token[41];
	uint moduleId;
	//bool tokenOk; // valid token  
};


#endif
