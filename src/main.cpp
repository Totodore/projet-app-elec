#include <Arduino.h>
#include <vector>
#include "./Bluetooth.cpp"
#include "./sensors/Mic.cpp"
#include "./screen/Screen.hpp"
#include "./sensors/Temperature.hpp"
#include "./sensors/Co2.cpp"

Bluetooth ble;
Screen screen;
Co2 co2;
Mic mic;
Temperature temp;

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting...");
	Serial.println("Configuration ended");
	pinMode(26, INPUT);
	screen.init();
	ble.init();
	mic.init();
	co2.init();
	temp.init();
}

void loop()
{
	ble.loop();
	co2.loop();
	mic.loop();
	temp.loop();
	screen.loop();
}
