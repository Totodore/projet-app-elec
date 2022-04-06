#include "sensors/Co2.hpp"

void Co2::init()
{
	Serial.println("Starting CO2 sensor...");
	pinMode(pin, INPUT);
	Serial.println("CO2 sensor started");
}

void Co2::loop()
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