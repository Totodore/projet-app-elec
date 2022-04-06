#include "sensors/Sensor.hpp"

bool Sensor::hasNewValue()
{
	return values.size() > 0;
}
int Sensor::getNewValue()
{
	int val = values.front();
	values.erase(values.begin());
	return val;
}