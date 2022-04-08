#pragma once
#include "BaseWorker.hpp"
#include "Arduino.h"

/**
 * Trame courante 19 octet : TRA(1) | OBJ(4) | REQ(1) | TYP(1) | NUM(2) | VAL(4) | TIM(4) | CHK(2)
 */
class Bluetooth : public BaseWorker
{
public:
	enum FRAME_TYPE
	{
		COURANTE = 0x31,
		SYNCHRO = 0x32,
		RAPIDE = 0x33,
	};
	enum QUERY_TYPE
	{
		READ = 0x31,
		WRITE = 0x32,
		RW = 0x33,
	};
	enum OBJECT_TYPE
	{
		HUMIDITY = 0x34,
		TEMPERATURE = 0x33,
		CO2 = 0x32,
		MIC = 0x41,
		CARD = 0x45,
	};
	enum OBJECT_NUM
	{
		HUMIDITY = "1",
		TEMPERATURE = "2",
		CO2 = "3",
		MIC = "4",
		CARD = "5",
	}
	void init();
	void loop();

private:
	uint8_t getChecksum(uint8_t *data, uint8_t len);
	void sendObjectFrame();
	void sendRapideFrame(uint8_t *data, size_t len);
	void sendSyncFrame();

private:
	unsigned long last_time = 0;
	static constexpr int interval = 2000; // Interval between print
	static constexpr char GROUP_NUMBER[] = "G01D";
};