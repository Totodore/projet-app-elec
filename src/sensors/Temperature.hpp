#pragma once
#include "../BaseWorker.hpp"
#include "Arduino.h"

#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 9
DHT dht(DHTPIN, DHTTYPE);

class Temperature : public BaseWorker
{
public:
	void init()
	{
		Serial.println("Starting capteur temperature...");
		pinMode(DHTPIN, INPUT);
		dht.begin();
		Serial.println("Capteur de temperature demarré");
	}
	void loop()
	{
		if (millis() - last_time < interval)
			return;
		last_time = millis();

		if (millis() - last_print_time > print_interval)
		{
			Serial.println("Temperature : " + String(dht.readTemperature() - 3) + " °C");
			Serial.println("Humidité : " + String(dht.readHumidity()) + " %");
		}
	}

private:
	int value = 0;
	unsigned long last_time = millis(); // Last execution time
	unsigned long last_print_time = millis();
	static constexpr int interval = 500; // Interval between executions
	static constexpr int print_interval = 1000;
};