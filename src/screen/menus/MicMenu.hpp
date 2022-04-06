#pragma once

#include "../Menu.hpp"

class MicMenu : public Menu
{

public:
	void draw(OledScreen &g)
	{
		g.DisplayString(5, 54, "Environnement sonore");
	}
	void onPot(int pot, int value)
	{
	}
	void onSensorValue(int value)
	{
	}

private:
};
