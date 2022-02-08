#pragma once
#include <Ethernet.h>
#include <SPI.h>
#include <Arduino.h>
#include "ValueList.h"
//#include <ArduinoSTL.h>

enum Action {
	StatusTemperatureOk,
	StatusHeatingOn,
	StatusCoolingOn
};


class WebServer
{
	public:
		WebServer();
		WebServer(byte macAdress[], IPAddress &ipAdress);
		~WebServer();
		void Start();
		void WebSiteBuildLoop(ValueList valueLogger, Action currentStatus, float currentTemperature, unsigned int timesTurnedOnHeating, unsigned int timesTurnedOnCooling);
		int SetWebParameters();
	private:
		ValueList webParameter;
		byte *mac;
		IPAddress *ip;
	
		EthernetServer server;
};

