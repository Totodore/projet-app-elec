#pragma once
#include <vector>
#include "Arduino.h"
#include "BaseWorker.hpp"
#include "Sensor.hpp"
/**
 * https://www.sgxsensortech.com/content/uploads/2016/07/MiCS-VZ-89TE-V1.0.pdf
 * 6 -> 3 -> 3.3v
 * 3 -> 1 ou 2 -> GND
 * 1 -> 6 -> 24
 * Carte ampli droite
 */
class Co2 : public BaseWorker, public Sensor
{
public:
	void init();
	void loop();

private:
	int high_count = 0;
	int low_count = 0;
	bool is_high = false;
	unsigned short int cycle = 0;
	unsigned short int ppm = 0;
	unsigned long last_time = millis(); // Last execution time
	static constexpr int pin = 24;			// CO2 Input pin
	static constexpr unsigned short int interval = 1000;
};