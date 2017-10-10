
#include "EIoT_Init.h"
#include "EEPROM_Helper.h"
#include "EIoTCloudRestApiV1.0.h"
#include <OneWire.h>
#include <DallasTemperature.h>



ADC_MODE(ADC_VCC); //vcc read-mode
EIoTCloudRestApi eiotcloud;

#define ONE_WIRE_BUS 2  // DS18B20 pin
#define LED_PIN 15  // 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);


StoreStruct storage = {
	CONFIG_VERSION,
	// token
	"1234567890123456789012345678901234567890",
	// The default module 0 - invalid module
	0,
	//0 // not valid
};

String moduleId = "";
String parameterId = "";
String parameterId2 = "";
float tempOld = 0;
float vccOld = 0;


void setup() {
	Serial.begin(115200);
	DEBUG_PRINTLN("Start...");
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, HIGH);
	loadConfig(@storage);

	eiotcloud.begin(AP_USERNAME, AP_PASSWORD);

	// if first time get new token and register new module
	// here hapend Plug and play logic to add module to Cloud
	if (storage.moduleId == 0)
	{
		createNewConfig(storage, eiotcloud);
		// save configuration
		saveConfig(storage);
	}

	// if something went wrong, wiat here
	if (storage.moduleId == 0)
		delay(1);

	// read module ID from storage
	moduleId = String(storage.moduleId);
	// read token ID from storage
	eiotcloud.SetToken(storage.token);
	// read Sensor.Parameter1 ID from cloud
	parameterId = eiotcloud.GetModuleParameterByName(moduleId, "Sensor.Parameter1");
	parameterId2 = eiotcloud.GetModuleParameterByName(moduleId, "Sensor.Parameter2");
	DEBUG_PRINT("parameterId: ");
	DEBUG_PRINTLN(parameterId);
}


void loop() {
	float temp;

	//do {
	//	DS18B20.requestTemperatures();
	//	temp = DS18B20.getTempCByIndex(0);
	//	DEBUG_PRINT("Temperature: ");
	//	DEBUG_PRINTLN(temp);
	//} while (temp == 85.0 || temp == (-127.0));

	float vcc = ESP.getVcc() / 851.5;
	DEBUG_PRINT("Vcc: ");
	DEBUG_PRINTLN(vcc);


	if (tempOld != vcc)
	{
		// send temperature
		bool valueRet = eiotcloud.SetParameterValue(parameterId, String(vcc));
		DEBUG_PRINT("SetParameterValue: ");
		DEBUG_PRINTLN(valueRet);
		tempOld = vcc;
	}
	if (vccOld != vcc)
	{
		// send temperature
		bool valueRet = eiotcloud.SetParameterValue(parameterId2, String(vcc));
		DEBUG_PRINT("SetParameterValue2: ");
		DEBUG_PRINTLN(valueRet);
		vccOld = vcc;
	}

	for (int i = 0; i < REPORT_INTERVAL; i++) {
		for (int j = 0; j < 15; j++) {
			digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
			delay(100);              // wait for a second
			digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
			delay(3900);              // wait for a second 
		}
	}

}

