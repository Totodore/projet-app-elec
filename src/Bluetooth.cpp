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

	uint8_t data[20];	// 19 bytes is the max size for a packet + null terminator
	data[19] = 0;
	if (Serial1.available())
	{
		Serial1.readBytes((char*) &data[0], 19);
		Serial.write((char*) &data[0]);
	}
	if (Serial.available())
	{
		Serial1.write(Serial.read());
	}
	// TODO: Handle communications with gateway
}

/**
 * @param time should be a pointer to a 4 bytes memory
 * @brief convert the current timestampt of seconds to a string 'mm:ss' and store it in the time pointer
 */
void Bluetooth::writeCurrentTime(uint8_t *time)
{
	unsigned long seconds = millis() / 1000;
	// Convert current_timestamp of seconds to a ascii string mm:ss
	sprintf((char *)time, "%02d%02d", (int)(seconds / 1000) % 60, (int)(seconds / 1000) / 60);
	Serial.println("Current time: " + String(*time));
}

/**
 * @param data should be a non null pointer
 * @param len should be the length of the data
 * @returns Generate a 2 byte checksum from the given data
 */
uint16_t Bluetooth::getChecksum(const uint8_t *data, size_t len)
{
	// Modulus sum of all bytes
	uint16_t checksum = 0;
	for (unsigned int i = 0; i < len; i++)
		checksum += data[i];
	return checksum % 0xFFFF;
}

/**
 * @param data should be a non null pointer to a 4 bytes memory
 * @brief Write the group number in the given data
 */
void Bluetooth::writeGroupNumber(uint8_t *data)
{
	// Copy the group number
	memcpy(data, GROUP_NUMBER, 4);
}

void Bluetooth::sendObjectFrame(OBJECT_TYPE object_type, OBJECT_NUM object_num, int value)
{
	static constexpr size_t len = 19;

	// Building object frame
	uint8_t data[len] = {};
	data[0] = Bluetooth::FRAME_TYPE::COURANTE; // TRA(1)
	writeGroupNumber(&data[1]);								 // OBJ(4)
	data[5] = Bluetooth::QUERY_TYPE::WRITE;		 // REQ(1)
	data[6] = object_type;										 // TYP(1)
	data[7] = object_num;											 // NUM(1)
	memcpy(&data[8], &value, 4);							 // VAL(4)
	writeCurrentTime(&data[12]);							 // TIM(4)
	uint16_t checksum = getChecksum(data, len - 2);
	memcpy(&data[16], &checksum, 2); // CHK(2)

	// Sending object frame
	Serial1.write(data, len);
}

void Bluetooth::sendRapideFrame(OBJECT_TYPE object_type, const uint8_t *data, uint8_t data_size)
{
	size_t len = 9 + data_size;

	// Building rapide frame
	uint8_t packet[len];
	memset(packet, 0, len);																						// Zero dynamic initialisation
	packet[0] = Bluetooth::FRAME_TYPE::RAPIDE;												// TRA(1)
	writeGroupNumber(&packet[1]);																			// OBJ(4)
	packet[5] = object_type;																					// TYP(1)
	packet[7] = data_size;																						// NBR(1)
	memcpy(&packet[8], data, data_size);															// DAT(NBR)
	uint16_t checksum = getChecksum(packet, len - 2);
	memcpy(&packet[8 + data_size], &checksum, 2); // CHK(2)

	// Sending rapide frame
	Serial1.write(packet, len);
}

void Bluetooth::sendSyncFrame()
{
	static constexpr size_t len = 2;

	uint8_t packet[len] = {};
	packet[0] = Bluetooth::FRAME_TYPE::SYNCHRO; // TRA(1)
	packet[1] = rand() % 255;										// BIN(1)

	Serial1.write(packet, len);
}

void Bluetooth::readFrame(uint8_t *data, size_t len)
{
	// Read the frame
	Serial1.readBytes((char *)data, len);

	if (getChecksum(data, len) != (data[len - 2] | (data[len - 1] << 8)))
	{
		Serial.println("Checksum error");
		return;
	}
}