#include <Arduino.h>
#include "OledScreen.hpp"
#include "data/logo.h"

OledScreen screen;

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting...");
	Serial1.begin(9600);
	Serial.println("Configuration ended");
	screen.InitI2C();
	screen.InitScreen();
	screen.Reset();
	screen.Display(logo);
	screen.DisplayString(30, 71, "MyEcovillage");
}

void loop()
{
	if (Serial1.available()) {
		Serial.write(Serial1.read());
	}
	if (Serial.available()) {
		Serial1.write(Serial.read());
	}
}
