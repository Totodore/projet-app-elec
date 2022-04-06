#include "sensors/Mic.hpp"
void Mic::init()
{
	Serial.println("Starting microphone...");
	pinMode(pin, INPUT);
	Serial.println("Microphone started");
}
void Mic::loop()
{
	// if (value == 0)
	value = analogRead(pin);
	Serial.println(value);
	// else
	// {
	// 	value += analogRead(pin);
	// 	value /= 2;
	// }
	// if (millis() - last_print_time > print_interval)
	// {
	// 	last_print_time = millis();
	// 	values.push_back(value);
	// 	value = 0;
	// }
}