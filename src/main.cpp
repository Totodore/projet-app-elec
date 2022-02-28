#include <Arduino.h>
#include "./Bluetooth.cpp"
#include "./Mic.cpp"
#include "./screen/Screen.cpp"

BluetoothWorker ble
Screen screen
MicWorker mic
void setup()
{
	Serial.begin(9600);
	Serial.println("Starting...");
	Serial.println("Configuration ended");
	ble.init();
	screen.init();
	mic.init();
}

void loop()
{
	ble.loop();
	screen.loop();
	mic.loop();
}
