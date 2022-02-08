#include "WebServer.h"




WebServer::WebServer() : server(80)
{
	
}

WebServer::WebServer(byte macAdress[], IPAddress &ipAdress) :server(80)
{
	mac = macAdress;
	ip = &ipAdress;
}


WebServer::~WebServer()
{
	
}


void WebServer::Start()
{	
	// You can use Ethernet.init(pin) to configure the CS pin
	//Ethernet.init(10);  // Most Arduino shields
	//Ethernet.init(5);   // MKR ETH shield
	//Ethernet.init(0);   // Teensy 2.0
	//Ethernet.init(20);  // Teensy++ 2.0
	//Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
	//Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

	if (Ethernet.begin(mac) == 1)
	{
		Serial.println(F("Connected

	}
	else 
	{
		delay(1000);
		Ethernet.begin(mac, *ip);
	}

	// Check for Ethernet hardware present
	if (Ethernet.hardwareStatus() == EthernetNoHardware)
	{
		Serial.println(F("Ethernet shield was not found.  Sorry, can't run without hardware. :("));
	}
	if (Ethernet.linkStatus() == LinkOFF)
	{
		Serial.println(F("Ethernet cable is not connected."));
	}
	

	if (Ethernet.linkStatus() == EthernetLinkStatus.LinkON)
	{
		Serial.println(Ethernet.localIP());
		Serial.print("subnet mask = ");
		Serial.println(Ethernet.subnetMask());
		Serial.print("gateway = ");
		Serial.println(Ethernet.gatewayIP());
		Serial.print("dns = ");
		Serial.println(Ethernet.dnsServerIP());


		// start the server
		server.begin();
		Serial.print(F("server is at "));
		Serial.println(Ethernet.localIP());
	}
}

void WebServer::WebSiteBuildLoop(ValueList valueLogger, Action currentStatus, float currentTemperature, unsigned int timesTurnedOnHeating, unsigned int timesTurnedOnCooling)
{

	// listen for incoming clients
	EthernetClient client = server.available();
	if (client)
	{

		// an http request ends with a blank line
		boolean currentLineIsBlank = true;
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				Serial.write(c);

				// if you've gotten to the end of the line (received a newline
				// character) and the line is blank, the http request has ended,
				// so you can send a reply
				if (c == '\n' && currentLineIsBlank) {
					// send a standard http response header
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println("Connection: close");  // the connection will be closed after completion of the response
					client.println("Refresh: 5");  // refresh the page automatically every 5 sec
					client.println();
					client.println(F("<!DOCTYPE HTML>"));
					client.println(F("<html >"));
					client.println("<body style=\"background-color:DarkSlateGray; \">");
					// output the value of each analog input pin
					client.println(F("<div align=\"center\" style=\"font-family:verdana; color:white; border-radius: 8px; background-color:black; margin:30; \" >"));
					client.println("<br />"); 
					client.println(F("<h1> Robbie en Gijzer bier project </h1>"));
					client.println("<br />");
					client.println("<br />");

					client.print("Current temp: ");
					client.print(currentTemperature);
					client.println("<br />");

					client.print("Avarage temp: ");
					client.print(valueLogger.avarage);
					client.println("<br />");

					client.print("lowest temp: ");
					client.print(valueLogger.lowestValue);
					client.println("<br />");

					client.print("highest temp: ");
					client.print(valueLogger.highestValue);
					client.println("<br />");
					client.println("<br />");

					switch (currentStatus)
					{
						case StatusCoolingOn:
							client.println(F("Cooling on"));
							break;
						case StatusHeatingOn:
							client.println(F("Heating on"));
							break;
						default:
							client.println(F("Temperature ok Cooling & Heating off"));
							break;
					}

					client.println("<br />");
					client.println("<br />");
					client.print("times turned on heating: ");
					client.print(timesTurnedOnHeating);
					client.println("<br />");

					client.print("times turned on cooling: ");
					client.print(timesTurnedOnCooling);
					client.println("<br />");

					client.println("<br />");

					for (int i = 0; i < valueLogger.size; i++)
					{
						client.print("Temperature ");
						client.print(String(i));
						client.print(" is: ");
						client.print(valueLogger.GetValue(i));
						client.println("<br />");
					}
					client.println("<br />");
					client.println("</ div >");
					client.println("</body>");
					client.println("</html>");
					break;
				}
				if (c == '\n') {
					// you're starting a new line
					currentLineIsBlank = true;
				}
				else if (c != '\r') {
					// you've gotten a character on the current line
					currentLineIsBlank = false;
				}
			}
		}
		// give the web browser time to receive the data
		delay(1);
		// close the connection:
		client.stop();
		//Serial.println("client disconnected");
	}
}


int WebServer::SetWebParameters()
{
	
	return 0;
}
