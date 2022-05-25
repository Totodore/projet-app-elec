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
	float value = 0;
	float low_average = 0;
	float high_average = 0;
	u_short counter = 0;
	std::vector<float> buffer;
	u_long last_print_time = millis();
	static constexpr int pin = 26;			 // Mic PIN
	static constexpr int led_pin = 25;		 // Mic PIN
	static constexpr int print_interval = 1000; // Interval between print
};