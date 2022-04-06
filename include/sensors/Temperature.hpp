#pragma once

#include "Arduino.h"
#include "DHT.h"

#include "BaseWorker.hpp"
#include "Sensor.hpp"

#define DHTTYPE DHT11
#define DHTPIN 9


class Temperature : public BaseWorker, public Sensor
{
public:
	void init();
	void loop();
	bool hasNewHumidityValue();
	int getNewHumidityValue();

private:
	int value = 0;
	unsigned long last_time = millis(); // Last execution time
	unsigned long last_print_time = millis();
	std::vector<int> humidityValues;
	static constexpr int interval = 500; // Interval between executions
	static constexpr int print_interval = 1000;
};
