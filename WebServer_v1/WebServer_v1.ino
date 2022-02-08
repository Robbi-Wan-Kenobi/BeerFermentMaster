#include <SPI.h>
#include <Ethernet.h>
#include "WebServer.h"
#include "ValueList.h"
#include <ArduinoSTL.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// https://sites.google.com/site/coolembeddedlaboratory/home/softwares/using-dhcp-mode-with-arduino-and-ethernet-shield

// Data output of DS18B20 is connected to Arduino digital pin 2
#define ONE_WIRE_BUS 2
#define PINNR_HEAT_RELAY 6
#define PINNR_COOL_RELAY 5
#define ETH_CS    10
#define SD_CS  4

const bool debugMode = false;

// constants won't change:
const long interval = 2000;
const float temperatureForHeating = 21.0;
const float temperatureForCooling = 22.2;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

EthernetClient client;

// Setting a one wire instance
OneWire oneWire(ONE_WIRE_BUS);
// Passing onewire instance to Dallas Temperature sensor library
DallasTemperature sensors(&oneWire);


ValueList valueLogger;
ValueList avarageTemp;

Action currentStatus = StatusTemperatureOk;
Action oldtatus = StatusTemperatureOk;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
static byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x76, 0xFD };
static IPAddress ip(192, 168, 0, 35);
WebServer webServer = WebServer(mac, ip);

float currentTemperature = 0;
float averagedTemperature = 0;
unsigned int timesTurnedOnCooling = 0;
unsigned int timesTurnedOnHeating = 0;

unsigned long currentMilli;

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
	

	// Open serial communications and wait for port to open:
	Serial.begin(9600);
	valueLogger = ValueList(30);
	avarageTemp = ValueList(5);

	//sensors.begin();                        // Begin the DS18B20 initialization
	pinMode(PINNR_HEAT_RELAY, OUTPUT);
	pinMode(PINNR_COOL_RELAY, OUTPUT);
	digitalWrite(PINNR_HEAT_RELAY, HIGH);
	digitalWrite(PINNR_COOL_RELAY, HIGH);

	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}

	pinMode(ETH_CS, OUTPUT);
	pinMode(SD_CS, OUTPUT);
	digitalWrite(ETH_CS, LOW); // Select the Ethernet Module.
	digitalWrite(SD_CS, HIGH); // De-Select the internal SD Card

	if (Ethernet.begin(mac) == 0)  // Start in DHCP Mode
	{
		Serial.println("Failed to configure Ethernet using DHCP, using Static Mode");
		// If DHCP Mode failed, start in Static Mode
		Ethernet.begin(mac, ip);
	}

	Serial.print("My IP address: ");

	for (byte thisByte = 0; thisByte < 4; thisByte++)
	{
		// print the value of each byte of the IP address:
		Serial.print(Ethernet.localIP()[thisByte], DEC);
		Serial.print(".");
	}
	Serial.println();

	// give the Ethernet shield a second to initialize:
	delay(1000);
	Serial.println("connecting...");

	// if you get a connection, report back via serial:
	if (client.connect(server, 5000))
	{
		// start the Ethernet connection and the server:
		if (!debugMode)
		{
			webServer.Start();
		}
		Serial.println("connected");
	}
	else
	{
		// if you didn't get a connection to the server:
		Serial.println("connection failed");
	}
}


	



void loop() {

	// if there are incoming bytes available 
	// from the server, read them and print them:
	if (client.available())
	{
		char c = client.read();
		client.write(c);
		Serial.print(c);
	}

	// as long as there are bytes in the serial queue,
 // read them and send them out the socket if it's open:
	while (Serial.available() > 0)
	{
		char inChar = Serial.read();
		if (client.connected())
		{
			client.print(inChar);
		}
	}

	// if the server's disconnected, stop the client:
	if (!client.connected()) {
		Serial.println();
		Serial.println("disconnecting.");
		client.stop();
		// do nothing:
	}

	currentMillis = millis();

	if (currentMillis - previousMillis >= interval)
	{
		// save the last time you blinked the LED
		previousMillis = currentMillis;

		sensors.requestTemperatures(); //Call all sensors on one wire to start calculating the temperature readings
		currentTemperature = sensors.getTempCByIndex(0);

		avarageTemp.AddValue(currentTemperature); //Get temperature reading from sensor 0 in celsius scale
		averagedTemperature = avarageTemp.avarage; // Gets tha avarage of all the buffered values

		valueLogger.AddValue(averagedTemperature);

		Serial.print("Averaged Temperature: ");
		Serial.println(averagedTemperature);

		currentStatus = StatusTemperatureOk;

		if (averagedTemperature >= temperatureForCooling)
		{
			currentStatus = StatusCoolingOn;
		}

		if (averagedTemperature <= temperatureForHeating)
		{
			currentStatus = StatusHeatingOn;
		}


		if (currentStatus != oldtatus)
		{
			oldtatus = currentStatus;
			switch (currentStatus)
			{
				case StatusTemperatureOk:
					digitalWrite(PINNR_COOL_RELAY, HIGH);
					digitalWrite(PINNR_HEAT_RELAY, HIGH);
					Serial.println(F("cooling & heatink off"));
					break;
				case StatusCoolingOn:
					digitalWrite(PINNR_COOL_RELAY, LOW);
					Serial.println(F("cooling on"));
					timesTurnedOnCooling += 1;
					break;
				case StatusHeatingOn:
					digitalWrite(PINNR_HEAT_RELAY, LOW);
					Serial.println(F("heating on"));
					timesTurnedOnHeating += 1;
					break;
			}
		}
		
	}
	if (!debugMode)
	{
		webServer.WebSiteBuildLoop(valueLogger, currentStatus, currentTemperature, timesTurnedOnHeating, timesTurnedOnCooling);
	}
}




