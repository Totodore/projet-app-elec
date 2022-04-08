#include "Bluetooth.hpp"
void Bluetooth::init()
{
	Serial.println("Starting bluetooth...");
	Serial1.begin(9600);
	Serial1.println("AT");
	Serial.println("Bluetooth started");
}

void Bluetooth::loop()
{
	if (millis() - last_time < interval)
		return;
	last_time = millis();

	if (Serial1.available())
	{
		Serial.write(Serial1.read());
	}
	if (Serial.available())
	{
		Serial1.write(Serial.read());
	}
	// TODO: Handle communications with gateway
}

void Bluetooth::sendObjectFrame(OBJECT_TYPE object_type, OBJECT_NUM object_num, int value) {
	// Size of an object frame
	uint8_t data[19] = {};
	data[0] = Bluetooth::FRAME_TYPE::COURANTE;
	for (int i = 1; i < 5; i++)
		data[i] = GROUP_NUMBER[i];
	data[5] = Bluetooth::QUERY_TYPE::WRITE;
	data[6] = object_type;
	data[7] = object_num;
	data[8] = value;
	
	
}