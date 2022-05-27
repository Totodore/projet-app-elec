#include "sensors/Co2.hpp"

void Co2::init()
{
	Serial.println("Starting CO2 sensor...");
	pinMode(pin, INPUT);
	Serial.println("CO2 sensor started");
}

void Co2::loop()
{
	// We get the value of the sensor (between 0 and 4096)
	int val = analogRead(pin);

	bool high;
	// If the value if above 3500 we are in high cycle, if it is under 3000 we are in low cycle
	if ((high = (val > 3500)))
		high_count++;
	else if (val < 3000)
		low_count++;
	else
		return;

	// If we pass from a low cycle to to a high cycle 
	if (high && !is_high)
	{
		cycle++;
		is_high = true;
	}
	// If we pass from a low cycle to a high cycle 
	else if (!high && is_high)
		is_high = false;

	/**
	 * Every 100 value (1s) we get the number of high values in the square signal
	 */
	if (cycle >= 10)
	{
		// Convert from cycle ratio to ppm with 25 as offset
		int ratio = (high_count / (float)(low_count + high_count)) * 100 + 25;
		int currentPpm = 40 * ratio - 1800;
		ppm += currentPpm;
		// If it is not the first value we make a mean
		if (ppm != currentPpm)
			ppm /= 2;

		high_count = low_count = cycle = 0;
		is_high = false;
	}
	// Every to sec we send the mean value
	if (millis() - last_time >= interval && isShown())
	{
		last_time = millis();
		values.push_back(ppm);
		high_count = low_count = cycle = ppm = 0;
	}
}