// 
// 
// 

#include "EEPROM_Helper.h"
#include <EEPROM.h>


void loadConfig(struct StoreStruct *storage, int size) {
	EEPROM.begin(512);
	// To make sure there are settings, and they are YOURS!
	// If nothing is found it will use the default settings.
	if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
		EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
		EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2])
		for (unsigned int t = 0; t<size; t++)
			*((char*)storage + t) = EEPROM.read(CONFIG_START + t);
	DEBUG_PRINT("Loaded ModuleId: ");
	DEBUG_PRINTLN(storage->moduleId);
}

void saveConfig(struct StoreStruct storage) {
	DEBUG_PRINT("Saved ModuleId: ");
	DEBUG_PRINTLN(storage.moduleId);
	EEPROM.begin(512);
	for (unsigned int t = 0; t<sizeof(storage); t++)
		EEPROM.write(CONFIG_START + t, *((char*)&storage + t));

	EEPROM.commit();
}

