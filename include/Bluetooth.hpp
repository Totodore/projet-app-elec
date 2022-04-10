#pragma once
#include <cstdlib>

#include "BaseWorker.hpp"
#include "Arduino.h"

/**
 * Trame courante 19 octet : TRA(1) | OBJ(4) | REQ(1) | TYP(1) | NUM(2) | VAL(4) | TIM(4) | CHK(2)
 * Trame rapide 9 + NBR octet : TRA(1) | OBJ(4) | TYP(1) | NBR(1) | DAT(NBR) | CHK(2)
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
		N_HUMIDITY,
		N_TEMPERATURE,
		N_CO2,
		N_MIC,
		N_CARD
	};

	void init();
	void loop();

private:
	uint16_t getChecksum(const uint8_t *data, size_t len);
	void writeCurrentTime(uint8_t *data);
	void writeGroupNumber(uint8_t *data);
	
	void sendObjectFrame(OBJECT_TYPE object_type, OBJECT_NUM object_num, int value);
	void sendRapideFrame(OBJECT_TYPE object_type, const uint8_t *data, uint8_t data_size);
	void sendSyncFrame();

	void readFrame(uint8_t *data, size_t len); 

private:
	unsigned long last_time = 0;
	static constexpr int interval = 2000; // Interval between print
	static constexpr char GROUP_NUMBER[] = "G01D";
};