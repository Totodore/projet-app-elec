#include "sensors/Mic.hpp"
void Mic::init()
{
	Serial.println("Starting microphone...");
	pinMode(pin, INPUT);
	pinMode(led_pin, OUTPUT);
	Serial.println("Microphone started");
}
void Mic::loop()
{
	digitalWrite(14, HIGH);

	double tension = ((analogRead(pin) - 2048) * 3.3) / 4096;
	if (tension < 0)
	{
		low_average += tension;
		low_average /= 2;
	}
	if (tension > 0)
	{
		high_average += tension;
		high_average /= 2;
	}
	if (tension < 0.00001)
		return;
	counter++;

	if (counter > 50)
	{
		float p2p = high_average - low_average;
		// https://forums.adafruit.com/viewtopic.php?f=8&t=100462
		int val = 20 * log10(p2p / 0.00631) + 94 - 44 - 25;
		digitalWrite(led_pin, val > 60 ? HIGH : LOW);
		value += val;
		value /= 2;
		counter = low_average = high_average = 0;
	}

	if (millis() - last_print_time > print_interval && isShown())
	{
		last_print_time = millis();
		values.push_back(value);
	}
}