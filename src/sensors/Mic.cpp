#include "sensors/Mic.hpp"
void Mic::init()
{
	Serial.println("Starting microphone...");
	pinMode(pin, INPUT);
	Serial.println("Microphone started");
}
void Mic::loop()
{
	int temp = analogRead(pin);
	value += abs(temp - 2048) / 4095.0 * 3.3;
	value /= 2;
	if (millis() - last_print_time > print_interval)
	{
		last_print_time = millis();
		values.push_back(10 * log10(value / 3.3) + 70);
		value = 0;
	}
}