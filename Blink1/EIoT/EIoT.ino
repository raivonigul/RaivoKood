
//#include <ESP8266WiFi.h>
#include "EIoT_Init.h"
//#include <EEPROM.h>
#include "EEPROM_Helper.h"
#include "EIoTCloudRestApiV1.0.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define DEBUG_PROG 

//#ifdef DEBUG_PROG
//#define DEBUG_PRINTLN(x)  Serial.println(x)
//#define DEBUG_PRINT(x)    Serial.print(x)
//#define REPORT_INTERVAL 1
//#else
//#define DEBUG_PRINTLN(x) 
//#define DEBUG_PRINT(x)
//#define REPORT_INTERVAL 10
//#endif

ADC_MODE(ADC_VCC); //vcc read-mode
EIoTCloudRestApi eiotcloud;

// change those lines
//#define AP_USERNAME "raivosuvila"
#define AP_USERNAME "NisuWiFi"
#define AP_PASSWORD "nigulikodu147"
//#define INSTANCE_ID "58bdd09747976c2bde23fa00"




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
	loadConfig(storage);

	eiotcloud.begin(AP_USERNAME, AP_PASSWORD);

	// if first time get new token and register new module
	// here hapend Plug and play logic to add module to Cloud
	if (storage.moduleId == 0)
	{
		createNewConfig(storage, eiotcloud);
		//// get new token - alternarive is to manually create token and store it in EEPROM
		//String token = eiotcloud.TokenNew(INSTANCE_ID);
		//DEBUG_PRINT("Token: ");
		//DEBUG_PRINTLN(token);
		//eiotcloud.SetToken(token);

		//// remember token
		//token.toCharArray(storage.token, 41);
		////***********************************************************
		//// add new module and configure it
		////**********************************************************
		//moduleId = eiotcloud.ModuleNew();
		//DEBUG_PRINT("ModuleId: ");
		//DEBUG_PRINTLN(moduleId);
		//storage.moduleId = moduleId.toInt();

		//// set module type
		//bool modtyperet = eiotcloud.SetModulType(moduleId, "MT_GENERIC");
		//DEBUG_PRINT("SetModulType: ");
		//DEBUG_PRINTLN(modtyperet);

		//// set module name
		//bool modname = eiotcloud.SetModulName(moduleId, "Room temperature");
		//DEBUG_PRINT("SetModulName: ");
		//DEBUG_PRINTLN(modname);
		////*************************************************
		//// Settings.Icon
		////************************************************
		//// add image settings parameter
		//String parameterImgId = eiotcloud.NewModuleParameter(moduleId, "Settings.Icon1");
		//DEBUG_PRINT("parameterImgId: ");
		//DEBUG_PRINTLN(parameterImgId);

		//// set module image
		//bool valueRet = eiotcloud.SetParameterValue(parameterImgId, "temperature.png");
		//DEBUG_PRINT("SetParameterValue: ");
		//DEBUG_PRINTLN(valueRet);

		////*************************************************
		//// Sensor.Parammeter1
		////************************************************
		//// now add parameter to display temperature
		//parameterId = eiotcloud.NewModuleParameter(moduleId, "Sensor.Parameter1");
		//DEBUG_PRINT("ParameterId: ");
		//DEBUG_PRINTLN(parameterId);

		////set parameter description
		//valueRet = eiotcloud.SetParameterDescription(parameterId, "Temperature");
		//DEBUG_PRINT("SetParameterDescription: ");
		//DEBUG_PRINTLN(valueRet);

		////set unit
		//// see http://meyerweb.com/eric/tools/dencoder/ how to encode �C 
		//valueRet = eiotcloud.SetParameterUnit(parameterId, "%C2%B0C");
		//DEBUG_PRINT("SetParameterUnit: ");
		//DEBUG_PRINTLN(valueRet);

		////Set parameter LogToDatabase
		//valueRet = eiotcloud.SetParameterLogToDatabase(parameterId, true);
		//DEBUG_PRINT("SetLogToDatabase: ");
		//DEBUG_PRINTLN(valueRet);


		////SetAvreageInterval
		//valueRet = eiotcloud.SetParameterAverageInterval(parameterId, "10");
		//DEBUG_PRINT("SetAvreageInterval: ");
		//DEBUG_PRINTLN(valueRet);

		////*************************************************
		//// Sensor.Parammeter2
		////************************************************
		//// now add parameter to display temperature
		//parameterId = eiotcloud.NewModuleParameter(moduleId, "Sensor.Parameter2");
		//DEBUG_PRINT("ParameterId: ");
		//DEBUG_PRINTLN(parameterId);

		////set parameter description
		//valueRet = eiotcloud.SetParameterDescription(parameterId, "Battery voltage");
		//DEBUG_PRINT("SetParameterDescription: ");
		//DEBUG_PRINTLN(valueRet);

		////set unit
		//// see http://meyerweb.com/eric/tools/dencoder/ how to encode �C 
		//valueRet = eiotcloud.SetParameterUnit(parameterId, "V");
		//DEBUG_PRINT("SetParameterUnit: ");
		//DEBUG_PRINTLN(valueRet);

		////Set parameter LogToDatabase
		//valueRet = eiotcloud.SetParameterLogToDatabase(parameterId, true);
		//DEBUG_PRINT("SetLogToDatabase: ");
		//DEBUG_PRINTLN(valueRet);

		////SetAvreageInterval
		//valueRet = eiotcloud.SetParameterAverageInterval(parameterId, "10");
		//DEBUG_PRINT("SetAvreageInterval: ");
		//DEBUG_PRINTLN(valueRet);

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

	do {
		DS18B20.requestTemperatures();
		temp = DS18B20.getTempCByIndex(0);
		DEBUG_PRINT("Temperature: ");
		DEBUG_PRINTLN(temp);
	} while (temp == 85.0 || temp == (-127.0));

	float vcc = ESP.getVcc() / 851.5;
	DEBUG_PRINT("Vcc: ");
	DEBUG_PRINTLN(vcc);


	if (tempOld != temp)
	{
		// send temperature
		bool valueRet = eiotcloud.SetParameterValue(parameterId, String(temp));
		DEBUG_PRINT("SetParameterValue: ");
		DEBUG_PRINTLN(valueRet);
		tempOld = temp;
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

