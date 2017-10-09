
ADC_MODE(ADC_VCC); //vcc read-mode
//#include <WiFiUdp.h>
//#include <WiFiServer.h>
//#include <WiFiClientSecure.h>
//#include <WiFiClient.h>
//#include <ESP8266WiFiType.h>
//#include <ESP8266WiFiSTA.h>
//#include <ESP8266WiFiScan.h>
//#include <ESP8266WiFiMulti.h>
//#include <ESP8266WiFiGeneric.h>
//#include <ESP8266WiFiAP.h>
#include <ESP8266WiFi.h>

extern "C" {
#include "user_interface.h" // this is for the RTC memory read/write functions
}

#define DEBUG_PROG

#ifdef DEBUG_PROG
#define DEBUG_PRINTLN(x)  Serial.println(x)
#define DEBUG_PRINT(x)    Serial.print(x)
#define REPORT_INTERVAL 1
#else
#define DEBUG_PRINTLN(x) 
#define DEBUG_PRINT(x)
#define REPORT_INTERVAL 10
#endif

#define RTCMEMORYSTART 66
#define RTCMEMORYLEN 125
#define VCC_ADJ  1.0   // measure with your voltmeter and calculate that the number mesured from ESP is correct

#define COLLECT 17
#define SEND 66
#define SLEEPTIME 60*1*1000000

#define SPARKFUN_BATTERY 1



typedef struct {
	int magicNumber;
	int valueCounter;
} rtcManagementStruc;

rtcManagementStruc rtcManagement;

typedef struct {
	float battery;
	int other;
} rtcStore;

rtcStore rtcValues;

int i;
int buckets;
unsigned long startTime;

//WiFiClient client;


void setup() {
	Serial.begin(115200);
	DEBUG_PRINTLN();
	DEBUG_PRINTLN();
	DEBUG_PRINTLN("Booted ");


	rst_info *rsti;
	rsti = ESP.getResetInfoPtr();

	switch (rsti->reason) {

	case 5:
		DEBUG_PRINTLN(" from RTC-RESET (ResetInfo.reason = 5)");
		break;
	case 6:
		DEBUG_PRINTLN(" from POWER-UP (ResetInfo.reason = 6)");
		rtcManagement.magicNumber = COLLECT;
		rtcManagement.valueCounter = 0;
		break;
	}

	system_get_rst_info();

	buckets = (sizeof(rtcValues) / 4);
	if (buckets == 0) buckets = 1;
	// DEBUG_PRINT("Buckets ");
	//  DEBUG_PRINTLN(buckets);
	system_rtc_mem_read(64, &rtcManagement, 8);
	// DEBUG_PRINT("Magic Number ");
	//  DEBUG_PRINTLN(rtcManagement.magicNumber);

	// initialize System after first start
	if (rtcManagement.magicNumber != COLLECT && rtcManagement.magicNumber != SEND) {
		rtcManagement.magicNumber = COLLECT;
		rtcManagement.valueCounter = 0;
		system_rtc_mem_write(64, &rtcManagement, 8);
		DEBUG_PRINTLN("Initial values set");
		ESP.deepSleep(10, WAKE_RF_DISABLED);
	}
	if (rtcManagement.magicNumber == COLLECT) {   // Read sensor and store
		if (rtcManagement.valueCounter <= RTCMEMORYLEN / buckets) {
			DEBUG_PRINTLN("Sensor reads");

			rtcValues.battery = ESP.getVcc() * VCC_ADJ;
			rtcValues.other = rtcManagement.valueCounter;

			int rtcPos = RTCMEMORYSTART + rtcManagement.valueCounter * buckets;
			system_rtc_mem_write(rtcPos, &rtcValues, buckets * 4);
			system_rtc_mem_write(rtcPos, &rtcValues, 4);
			system_rtc_mem_write(64, &rtcManagement, 4);

			DEBUG_PRINT("Counter : ");
			DEBUG_PRINT(rtcManagement.valueCounter);
			DEBUG_PRINT(" Position: ");
			DEBUG_PRINT(rtcPos);
			DEBUG_PRINT(", battery: ");
			DEBUG_PRINT(rtcValues.battery);
			rtcManagement.valueCounter++;
			system_rtc_mem_write(64, &rtcManagement, 8);
			DEBUG_PRINTLN("before sleep W/O RF");
			ESP.deepSleep(SLEEPTIME, WAKE_NO_RFCAL);
		}
		else {    // set initial values
			rtcManagement.magicNumber = SEND;
			rtcManagement.valueCounter = 0;
			system_rtc_mem_write(64, &rtcManagement, 8);
			DEBUG_PRINTLN("before sleep w RF");
			ESP.deepSleep(10, WAKE_RFCAL);
		}
	}
	else {  // Send to Cloud
		startTime = millis();
		DEBUG_PRINTLN();
		DEBUG_PRINTLN();
		WiFi.mode(WIFI_STA);
		DEBUG_PRINT("Start Sending values. Connecting to ");
		//  DEBUG_PRINTLN(ASP_ssid);
		//  WiFi.begin(ASP_ssid, ASP_password);

		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
			DEBUG_PRINT(".");
		}
		DEBUG_PRINTLN("Sending ");

		for (i = 0; i <= RTCMEMORYLEN / buckets; i++) {
			int rtcPos = RTCMEMORYSTART + i * buckets;
			system_rtc_mem_read(rtcPos, &rtcValues, sizeof(rtcValues));
			DEBUG_PRINT("i: ");
			DEBUG_PRINT(i);
			DEBUG_PRINT(" Position ");
			DEBUG_PRINT(rtcPos);
			DEBUG_PRINT(", battery: ");
			DEBUG_PRINT(rtcValues.battery);
			DEBUG_PRINT(", other: ");
			DEBUG_PRINTLN(rtcValues.other);
			//sendSparkfun(SPARKFUN_BATTERY, rtcValues.other, rtcValues.battery);
			yield();
		}
		rtcManagement.magicNumber = COLLECT;
		rtcManagement.valueCounter = 0;
		system_rtc_mem_write(64, &rtcManagement, 8);
		DEBUG_PRINT("Writing to Cloud done. It took ");
		DEBUG_PRINT(millis() - startTime) / 1000.0;
		DEBUG_PRINTLN(" Seconds ");
		ESP.deepSleep(SLEEPTIME, WAKE_NO_RFCAL);
	}
}


void loop() {}
