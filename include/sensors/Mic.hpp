#pragma once
#include "BaseWorker.hpp"
#include "Arduino.h"

/**
 * Mic output 6 -> 26 tiva
 */
class Mic : public BaseWorker
{
public:
	void init();
	void loop();

private:
	int value = 0;
	unsigned long last_print_time = millis();
	static constexpr int pin = 26;			 // CO2 Input pin
	static constexpr int print_interval = 5000; // Interval between print
};