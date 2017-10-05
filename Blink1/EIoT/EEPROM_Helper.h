// EEPROM_Helper.h

#ifndef _EEPROM_HELPER_h
#define _EEPROM_HELPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include "EIoT.h"

#define CONFIG_START 0
#define CONFIG_VERSION "v02"


void loadConfig(StoreStruct storage);
void saveConfig(StoreStruct storage);

#endif

