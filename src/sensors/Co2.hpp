#pragma once
#include <vector>
#include "Arduino.h"
#include "../BaseWorker.hpp"
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
	void init()
	{
		Serial.println("Starting CO2 sensor...");
		pinMode(pin, INPUT);
		Serial.println("CO2 sensor started");
	}

	/**
	 * Valeur à exclure entre 3400 et 3500
	 */
	void loop()
	{
		int val = analogRead(pin);
		bool high;
		if ((high = (val > 3500)))
			high_count++;
		else if (val < 3000)
			low_count++;
		else
			return;

		if (high && !is_high)
		{
			cycle++;
			is_high = true;
		}
		else if (!high && is_high)
			is_high = false;

		/**
		 * Every 100 value (1s) we get the number of high values in the square signal
		 */
		if (cycle >= 10)
		{
			// Convert from cucle to ppm with 25 as offset
			int ratio = (high_count / (float)(low_count + high_count)) * 100 + 25;
			int currentPpm = 40 * ratio - 1800;
			ppm += currentPpm;
			// If it is not the first value we make a mean
			if (ppm != currentPpm)
				ppm /= 2;

			high_count = low_count = cycle = 0;
			is_high = false;
		}
		if (millis() - last_time >= interval)
		{
			last_time = millis();
			values.push_back(ppm);
			ppm = 0;
		}
	}

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