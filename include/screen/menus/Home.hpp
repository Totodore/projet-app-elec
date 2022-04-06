#pragma once

#include "screen/Menu.hpp"
#include "data/logo.hpp"

class Home : public Menu
{
public:
	void draw(OledScreen &g);
	void onPot(int pot, int value);
	void onSensorValue(int value);
};
