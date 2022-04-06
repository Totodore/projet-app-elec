#pragma once
#include "../Menu.hpp"
#include "../../data/logo.h"

class Home : public Menu
{
public:
	void draw(OledScreen &g)
	{
		g.Display(logo);
		g.DisplayString(30, 54, "MyEcovillage");
	}
	void onPot(int pot, int value)
	{
	}
	void onSensorValue(int value)
	{
	}
};
