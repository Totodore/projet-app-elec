#ifndef HOME_HPP
#define HOME_HPP

#include "../Menu.hpp"
#include "../../data/logo.h"

class Home : public Menu
{
public:
	void draw(OledScreen &g)
	{
		g.Display(logo);
		g.DisplayString(30, 71, "MyEcovillage");
	}
	void onPot(int pot, int value)
	{
		Serial.print("POT ");
		Serial.print(pot);
		Serial.print(": ");
		Serial.println(value);
	}
};

#endif // HOME_HPP