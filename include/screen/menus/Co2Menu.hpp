#pragma once

#include <vector>
#include "screen/Menu.hpp"

class Co2Menu : public Menu
{

public:
	void draw(OledScreen &g);
	void onPot(int pot, int value);
	void onSensorValue(int value);

private:
	std::vector<unsigned short int> values;
};
