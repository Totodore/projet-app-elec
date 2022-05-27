#pragma once

#include <vector>

class Sensor
{
public:
	bool hasNewValue();
	bool isShown();
	int getNewValue();

protected:
	std::vector<unsigned int> values;
};