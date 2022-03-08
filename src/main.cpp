#include <Arduino.h>
#include <vector>
#include "./Bluetooth.cpp"
#include "./sensors/Mic.cpp"
#include "./screen/Screen.hpp"
#include "./sensors/Co2.cpp"

Bluetooth ble;
Screen screen;
Co2 co2;
Mic mic;

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting...");
	Serial.println("Configuration ended");
	screen.init();
	ble.init();
	mic.init();
	co2.init();
	pinMode(27, INPUT);
	pinMode(28, INPUT);
	pinMode(29, INPUT);
}

void loop()
{
	ble.loop();
	co2.loop();
	mic.loop();
	screen.loop();
}
