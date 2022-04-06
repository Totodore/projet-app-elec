#pragma once
#include "../BaseWorker.hpp"
#include "Sensor.hpp"
#include "Arduino.h"

#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 9
DHT dht(DHTPIN, DHTTYPE);

class Temperature : public BaseWorker, public Sensor
{
public:
	void init()
	{
		Serial.println("Démarrage du capteur de temperature...");
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
			// Serial.println("Temperature : " + String(dht.readTemperature() - 3) + " °C");
			// Serial.println("Humidité : " + String(dht.readHumidity()) + " %");
			values.push_back(dht.readTemperature() - 3);
			humidityValues.push_back(dht.readHumidity());
		}
	}

	bool hasNewHumidityValue() {
		return humidityValues.size() > 0;
	}
	int getNewHumidityValue() {
		int val = humidityValues.front();
		humidityValues.erase(humidityValues.begin());
		return val;
	}

private:
	int value = 0;
	unsigned long last_time = millis(); // Last execution time
	unsigned long last_print_time = millis();
	std::vector<int> humidityValues;
	static constexpr int interval = 500; // Interval between executions
	static constexpr int print_interval = 1000;
};