#include <Arduino.h>
#include "OledScreen.hpp"
#include "data/logo.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2
#define DHTTYPE    DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup()
{
  Serial.begin(9600);
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  sensor_t sensor;
  Serial.println(F("------------------------------------"));
}
void loop() 
{
	delay(1000);
 	sensors_event_t event;
  	dht.temperature().getEvent(&event);

    Serial.print(F("Temperature: "));
    Serial.print(event.temperature - 5);
    Serial.println(F("°C"));

	dht.humidity().getEvent(&event);
	Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
}