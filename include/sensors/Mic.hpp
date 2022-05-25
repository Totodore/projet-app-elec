#pragma once
#include "BaseWorker.hpp"
#include "Arduino.h"
#include "Sensor.hpp"

#include <cmath>
#include <vector>

/**
 * Mic output 6 -> 26 tiva
 */
class Mic : public BaseWorker, public Sensor
{
public:
	void init();
	void loop();

private:
	double value = 0;
	std::vector<u_short> buffer;
	u_long last_print_time = millis();
	static constexpr int pin = 26;			 // CO2 Input pin
	static constexpr int print_interval = 1000; // Interval between print
};