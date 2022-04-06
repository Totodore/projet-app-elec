#pragma once
#include <string>

#include "../Menu.hpp"

class HumMenu : public Menu
{

public:
	void draw(OledScreen &g)
	{
		g.DisplayString(2, 54, "Humidite:");
		if (values.size() > 0)
		{
			g.DisplayString(105, 54, values.back());
			g.DisplayString(120, 54, "%");
		}
		drawGraph(g, 0, 100);
		for (unsigned short int i = 0; i < values.size(); i++)
		{
			int value = 92 - values[i] / 100.0 * 92;
			if (value > 92)
				value = 92;
			if (value < 0)
				value = 0;
			g.DrawPixel(i, value, false);
		}
		if (values.size() >= 127)
		{
			values.clear();
			g.Reset();
			setDirty(true);
		}
	}
	void onPot(int pot, int value)
	{
	}
	void onSensorValue(int value)
	{
		// Serial.print("Humidity value: ");
		// Serial.println(value);
		values.push_back(value);
		setDirty(true);
	}

private:
	std::vector<unsigned short int> values;
};
