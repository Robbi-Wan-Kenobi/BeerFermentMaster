# BeerFermentMaster
Temperature controller for fermenting beer

This is an Arduino Uno Project.

It Controls the temperature of fermenting beer 
that is placed in a refrigerator together with a heater, both controlled by this application
Together they regulate to the desired temperature. 

Aditionaly it serves a website that shows the temperature history.

It involves a
- OneWire Dallas DS18B20 temperature sensor 
- two output Relays For switching the freezer and heater
- Ardrino TCP IP Ethernet Shield

The webpage is only for diagnostics.
It shows the current, highest, lowest and avarage temperature
together with a historical list of temperatures

![BeerFermenter](https://user-images.githubusercontent.com/19152655/156921459-bcc95bba-d7a7-4ec9-b0f0-aabb09e5f5fe.png)



It is a Visual Studio project but you can also open it in the Ardrino IDE.

All parameters are controlled in WebServer_v1.ino 
- ONE_WIRE_BUS            Pin nr for onewire DS18B20 temperature sesnor
- PINNR_HEAT_RELAY        Pin nr for heater
- PINNR_COOL_RELAY        Pin nr for cooler
- temperatureForHeating   If temperature is below this value heating turns on
- temperatureForCooling   If temperature is below this value cooling turns on
- IPAddress ip            IP address of shield, there is a link included on how to configure DHCP

ValueList is a class that stores a configrable amount of floats, a rolling avarage is calculates after a new value is added.
