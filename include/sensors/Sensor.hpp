#pragma once

#include <vector>

class Sensor
{
public:
	bool hasNewValue();
	int getNewValue();

protected:
	std::vector<unsigned int> values;
};