#pragma once

class Sensor
{
public:
	bool hasNewValue() {
		return values.size() > 0;
	}
	int getNewValue() {
		int val = values.front();
		values.erase(values.begin());
		return val;
	}


protected:
	std::vector<unsigned int> values; 
};