// 
// 
// 

#include "EIoT.h"
#include "EIoT_Init.h"

void createNewConfig(struct StoreStruct *storage,  EIoTCloudRestApi eiotcloud)
{
	String token = eiotcloud.TokenNew(INSTANCE_ID);
	DEBUG_PRINT("Token: ");
	DEBUG_PRINTLN(token);
	eiotcloud.SetToken(token);

	// remember token
	token.toCharArray(storage->token, 41);
	//***********************************************************
	// add new module and configure it
	//**********************************************************
	String moduleId = eiotcloud.ModuleNew();
	DEBUG_PRINT("createNewConfig ModuleId: ");
	DEBUG_PRINTLN(moduleId);
	storage->moduleId = moduleId.toInt();
	DEBUG_PRINT("createNewConfig storage.moduleId: ");
	DEBUG_PRINTLN(storage->moduleId);

	// set module type
	bool modtyperet = eiotcloud.SetModulType(moduleId, "MT_GENERIC");
	DEBUG_PRINT("SetModulType: ");
	DEBUG_PRINTLN(modtyperet);

	// set module name
	bool modname = eiotcloud.SetModulName(moduleId, "Room temperature");
	DEBUG_PRINT("SetModulName: ");
	DEBUG_PRINTLN(modname);
	//*************************************************
	// Settings.Icon
	//************************************************
	// add image settings parameter
	String parameterImgId = eiotcloud.NewModuleParameter(moduleId, "Settings.Icon1");
	DEBUG_PRINT("parameterImgId: ");
	DEBUG_PRINTLN(parameterImgId);

	// set module image
	bool valueRet = eiotcloud.SetParameterValue(parameterImgId, "temperature.png");
	DEBUG_PRINT("SetParameterValue: ");
	DEBUG_PRINTLN(valueRet);

	//*************************************************
	// Sensor.Parammeter1
	//************************************************
	// now add parameter to display temperature
	String parameterId = eiotcloud.NewModuleParameter(moduleId, "Sensor.Parameter1");
	DEBUG_PRINT("ParameterId: ");
	DEBUG_PRINTLN(parameterId);

	//set parameter description
	valueRet = eiotcloud.SetParameterDescription(parameterId, "Temperature");
	DEBUG_PRINT("SetParameterDescription: ");
	DEBUG_PRINTLN(valueRet);

	//set unit
	// see http://meyerweb.com/eric/tools/dencoder/ how to encode �C 
	valueRet = eiotcloud.SetParameterUnit(parameterId, "%C2%B0C");
	DEBUG_PRINT("SetParameterUnit: ");
	DEBUG_PRINTLN(valueRet);

	//Set parameter LogToDatabase
	valueRet = eiotcloud.SetParameterLogToDatabase(parameterId, true);
	DEBUG_PRINT("SetLogToDatabase: ");
	DEBUG_PRINTLN(valueRet);


	//SetAvreageInterval
	valueRet = eiotcloud.SetParameterAverageInterval(parameterId, "10");
	DEBUG_PRINT("SetAvreageInterval: ");
	DEBUG_PRINTLN(valueRet);

	//*************************************************
	// Sensor.Parammeter2
	//************************************************
	// now add parameter to display temperature
	parameterId = eiotcloud.NewModuleParameter(moduleId, "Sensor.Parameter2");
	DEBUG_PRINT("ParameterId: ");
	DEBUG_PRINTLN(parameterId);

	//set parameter description
	valueRet = eiotcloud.SetParameterDescription(parameterId, "Battery voltage");
	DEBUG_PRINT("SetParameterDescription: ");
	DEBUG_PRINTLN(valueRet);

	//set unit
	// see http://meyerweb.com/eric/tools/dencoder/ how to encode �C 
	valueRet = eiotcloud.SetParameterUnit(parameterId, "V");
	DEBUG_PRINT("SetParameterUnit: ");
	DEBUG_PRINTLN(valueRet);

	//Set parameter LogToDatabase
	valueRet = eiotcloud.SetParameterLogToDatabase(parameterId, true);
	DEBUG_PRINT("SetLogToDatabase: ");
	DEBUG_PRINTLN(valueRet);

	//SetAvreageInterval
	valueRet = eiotcloud.SetParameterAverageInterval(parameterId, "10");
	DEBUG_PRINT("SetAvreageInterval: ");
	DEBUG_PRINTLN(valueRet);
}
