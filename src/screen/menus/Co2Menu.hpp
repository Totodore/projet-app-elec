#pragma once

#include "../Menu.hpp"
#include <vector>
class Co2Menu : public Menu
{

public:
	void draw(OledScreen &g) override
	{
		g.DisplayString(2, 54, "CO2:");
		if (values.size() > 0)
		{
			g.DisplayString(85, 54, values.back());
			g.DisplayString(110, 54, "ppm");
		}
		drawGraph(g, 0, 1500);
		for (unsigned short int i = 0; i < values.size(); i++)
		{
			int value = 92 - values[i] / 1500.0 * 92;
			if (value > 92)
				value = 92;
			if (value < 0)
				value = 0;
			g.DrawPixel(i, value, false);
		}
	}
	void onPot(int pot, int value) override
	{
	}
	void onSensorValue(int value)
	{
		Serial.print("Co2 value: ");
		Serial.println(value);
		values.push_back(value);
		setDirty(true);
	}

private:
	std::vector<unsigned short int> values;
};
