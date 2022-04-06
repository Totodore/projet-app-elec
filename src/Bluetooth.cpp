#include "Bluetooth.hpp"
void Bluetooth::init()
{
	Serial.println("Starting bluetooth...");
	Serial1.begin(9600);
	Serial1.println("AT");
	Serial.println("Bluetooth started");
}

void Bluetooth::loop()
{
	if (Serial1.available())
	{
		Serial.write(Serial1.read());
	}
	if (Serial.available())
	{
		Serial1.write(Serial.read());
	}
	// TODO: Handle communications with gateway
}