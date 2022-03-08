#include <vector>
#include "Arduino.h"
#include "BaseWorker.cpp"
/**
 * https://www.sgxsensortech.com/content/uploads/2016/07/MiCS-VZ-89TE-V1.0.pdf
 */
class Co2 : public BaseWorker
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
		if (millis() - last_time < interval)
			return;
		last_time = millis();
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
		{
			is_high = false;
		}

		/**
		 * Every 100 value (1s) we get the number of high values in the square signal
		 */
		if (cycle >= 10)
		{
			int ratio = (high_count / (float)(low_count + high_count)) * 100 + 25;
			int ppm = 40 * ratio - 1800;
			// Serial.print("CO2: ");
			Serial.println(ppm);
			// Serial.println(" ppm");
			high_count = low_count = cycle = 0;
			is_high = false;
		}
	}

private:
	int high_count = 0;
	int low_count = 0;
	bool is_high = false;
	int cycle = 0;
	unsigned long last_time = millis(); // Last execution time
	static constexpr int pin = 28;			// CO2 Input pin
	static constexpr int interval = 10; // Interval between executions
};