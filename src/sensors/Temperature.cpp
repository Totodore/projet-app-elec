#include "sensors/Temperature.hpp"

DHT dht(DHTPIN, DHTTYPE);

void Temperature::init()
{
	Serial.println("Démarrage du capteur de temperature...");
	pinMode(9, INPUT);
	dht.begin();
	Serial.println("Capteur de temperature demarré");
}
void Temperature::loop()
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

bool Temperature::hasNewHumidityValue()
{
	return humidityValues.size() > 0;
}
int Temperature::getNewHumidityValue()
{
	int val = humidityValues.front();
	humidityValues.erase(humidityValues.begin());
	return val;
}