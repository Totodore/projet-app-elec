#include <Arduino.h>
#include <vector>
// #include "./Bluetooth.cpp"
#include "./sensors/Mic.cpp"
// #include "./screen/Screen.cpp"
#include "./sensors/Co2.cpp"

// BluetoothWorker ble
// Screen screen
Co2 co2;
Mic mic;
// MICS_VZ_89TE CO2SENSOR;

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting...");
	Serial.println("Configuration ended");
	// ble.init();
	// screen.init();
	mic.init();
	co2.init();
}

void loop()
{
	// ble.loop();
	// screen.loop();
	// CO2SENSOR.readSensor();
	// Serial.println(CO2SENSOR.getCO2());
	// Serial.println(CO2SENSOR.getVOC());
	// Serial.print("A: ");
	// Serial.println(analogRead(29));
	// Serial.print("B: ");
	co2.loop();
	// mic.loop();

}
