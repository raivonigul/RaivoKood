
#include "EIoT_Init.h"
#include "EEPROM_Helper.h"
#include "EIoTCloudRestApiV1.0.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

extern "C" {
#include "user_interface.h" // this is for the RTC memory read/write functions
}




ADC_MODE(ADC_VCC); //vcc read-mode
EIoTCloudRestApi eiotcloud;

#define ONE_WIRE_BUS 2  // DS18B20 pin
#define LED_PIN 15  // 
#define SLEEPTIME 60*1*1000000

#ifdef DEBUG_PROG
#define DEBUG_LED_ON  digitalWrite(LED_PIN, HIGH)
#define DEBUG_LED_OFF  digitalWrite(LED_PIN, LOW)
#else
#define DEBUG_LED_ON 
#define DEBUG_LED_OFF
#endif


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

typedef struct {
	int vcc;
	int tempperature;
} rtcBufferStructure;


rtcBufferStructure rtcBuffer;

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
unsigned long startTime;


void setup() {
	startTime = millis();
	Serial.begin(115200);
	DEBUG_PRINTLN("Start...");
	pinMode(LED_PIN, OUTPUT);
	DEBUG_LED_ON;
	rst_info *rsti;
	rsti = ESP.getResetInfoPtr();

	system_rtc_mem_read(64, &rtcBuffer, 8);



	int temp;

	DS18B20.requestTemperatures();
	temp = 100 * DS18B20.getTempCByIndex(0);
	DEBUG_PRINT("Temperature: ");
	DEBUG_PRINT(rtcBuffer.tempperature);
	DEBUG_PRINT("-->");
	DEBUG_PRINTLN(temp);


	int vcc = ESP.getVcc();
	DEBUG_PRINT("Vcc: ");
	DEBUG_PRINT(rtcBuffer.vcc);
	DEBUG_PRINT("-->");
	DEBUG_PRINTLN(vcc);

	if (!(vcc == rtcBuffer.vcc && temp  == rtcBuffer.tempperature))
	{

		//start WiFi
		loadConfig(&storage, sizeof(storage));


		eiotcloud.begin(AP_USERNAME, AP_PASSWORD);

		// if first time get new token and register new module
		// here hapend Plug and play logic to add module to Cloud
		if (storage.moduleId == 0)
		{
			createNewConfig(&storage, eiotcloud);
			DEBUG_PRINT("createNewConfig returned storage.moduleId: ");
			DEBUG_PRINTLN(storage.moduleId);

			//save configuration
			saveConfig(storage);
		}

		// if something went wrong, wiat here
		if (storage.moduleId == 0)
			delay(1);

		// read module ID from storage
		moduleId = String(storage.moduleId);
		DEBUG_PRINT("moduleId: ");
		DEBUG_PRINTLN(moduleId);
		// read token ID from storage
		eiotcloud.SetToken(storage.token);
		// read Sensor.Parameter1 ID from cloud
		parameterId = eiotcloud.GetModuleParameterByName(moduleId, "Sensor.Parameter1");
		parameterId2 = eiotcloud.GetModuleParameterByName(moduleId, "Sensor.Parameter2");
		DEBUG_PRINT("parameterId: ");
		DEBUG_PRINTLN(parameterId);


		// send temperature
		float vccf = vcc / 851.5;
		float tempf = temp / 100.0;
		bool valueRet = eiotcloud.SetParameterValue(parameterId, String(tempf));
		DEBUG_PRINT("Set Temperature: ");
		DEBUG_PRINTLN(valueRet);

		// send temperature
		valueRet = eiotcloud.SetParameterValue(parameterId2, String(vccf));
		DEBUG_PRINT("Set Vcc: ");
		DEBUG_PRINTLN(valueRet);

		//save to buffer
		rtcBuffer.vcc = vcc;
		rtcBuffer.tempperature = temp;
		system_rtc_mem_write(64, &rtcBuffer, 8);


	}


	//deepleep
	DEBUG_PRINT("Writing to Cloud done. It took ");
	DEBUG_PRINT(millis() - startTime) / 1000.0;
	DEBUG_PRINTLN(" Seconds ");
	DEBUG_LED_OFF;
	ESP.deepSleep(SLEEPTIME, WAKE_NO_RFCAL);

}


void loop() {
	//int temp;

	//do {
	//	DS18B20.requestTemperatures();
	//	temp = DS18B20.getTemp(0);
	//	DEBUG_PRINT("Temperature: ");
	//	DEBUG_PRINTLN(temp);
	//} while (temp == 85.0 || temp == (-127.0));

	//float vcc = ESP.getVcc() / 851.5;
	//DEBUG_PRINT("Vcc: ");
	//DEBUG_PRINTLN(vcc);


	//if (tempOld != vcc)
	//{
	//	// send temperature
	//	bool valueRet = eiotcloud.SetParameterValue(parameterId, String(vcc));
	//	DEBUG_PRINT("SetParameterValue: ");
	//	DEBUG_PRINTLN(valueRet);
	//	tempOld = vcc;
	//}
	//if (vccOld != vcc)
	//{
	//	// send temperature
	//	bool valueRet = eiotcloud.SetParameterValue(parameterId2, String(vcc));
	//	DEBUG_PRINT("SetParameterValue2: ");
	//	DEBUG_PRINTLN(valueRet);
	//	vccOld = vcc;
	//}

	//for (int i = 0; i < REPORT_INTERVAL; i++) {
	//	for (int j = 0; j < 5; j++) {
	//		digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
	//		delay(100);              // wait for a second
	//		digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
	//		delay(3900);              // wait for a second 
	//	}
	//}
}

