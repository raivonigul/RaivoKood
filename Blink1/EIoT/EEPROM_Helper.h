// EEPROM_Helper.h

#ifndef _EEPROM_HELPER_h
#define _EEPROM_HELPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define CONFIG_START 0
#define CONFIG_VERSION "v02"

struct StoreStruct {
	// This is for mere detection if they are your settings
	char version[4];
	// The variables of your settings
	char token[41];
	uint moduleId;
	//bool tokenOk; // valid token  
};

void loadConfig(StoreStruct storage);
void saveConfig(StoreStruct storage);

#endif

