#pragma once
#include "BaseWorker.hpp"
#include "Arduino.h"
class Bluetooth : public BaseWorker
{
public:
	void init();
	void loop();
};