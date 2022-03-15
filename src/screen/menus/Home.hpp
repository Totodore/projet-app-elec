#pragma once
#include "../Menu.hpp"
#include "../../data/logo.h"

class Home : public Menu
{
public:
	void draw(OledScreen &g)
	{
		g.Display(logo);
		g.DisplayString(30, 71, "MyEcovillage");
		g.DisplayString(0, 90, pot1);
	}
	void onPot(int pot, int value)
	{
		Serial.print("POT ");
		Serial.print(pot);
		Serial.print(": ");
		Serial.println(value);
	}

private:
	short int pot1 = 0;
};
