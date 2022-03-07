#include "BaseWorker.cpp"
#include "Arduino.h"
class Mic : public BaseWorker
{
public:
	void init()
	{
		Serial.println("Starting microphone...");
		pinMode(pin, INPUT);
		Serial.println("Microphone started");
	}
	void loop()
	{
		if (millis() - last_time < interval)
			return;
		last_time = millis();
		if (value == 0)
			value = analogRead(pin);
		else
		{
			value += analogRead(pin);
			value /= 2;
		}
		if (millis() - last_print_time > print_interval)
		{
			Serial.print("Microphone: ");
			Serial.println(value);
		}
	}

private:
	int value = 0;
	unsigned long last_time = millis(); // Last execution time
	unsigned long last_print_time = millis();
	static constexpr int pin = 28;			 // CO2 Input pin
	static constexpr int interval = 100; // Interval between executions
	static constexpr int print_interval = 1000;
};