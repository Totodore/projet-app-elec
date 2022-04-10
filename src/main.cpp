#include <Arduino.h>
#include <vector>

#include "./BaseWorker.hpp"
#include "./Bluetooth.hpp"
#include "./sensors/Mic.hpp"
#include "./sensors/Temperature.hpp"
#include "./sensors/Co2.hpp"
#include "./screen/Screen.hpp"

std::vector<BaseWorker *> workers;

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting...");

	Bluetooth *ble = new Bluetooth();
	Mic *mic = new Mic();
	Temperature *temp = new Temperature();
	Co2 *co2 = new Co2();
	Screen *screen = new Screen(temp, co2, mic, ble);
	workers.push_back(ble);
	workers.push_back(mic);
	workers.push_back(temp);
	workers.push_back(co2);
	workers.push_back(screen);

	for (auto worker : workers)
		worker->init();
	Serial.println("Configuration ended");
}

void loop()
{

	for (auto worker : workers)
		worker->loop();
}
