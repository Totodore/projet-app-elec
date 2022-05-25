#include "screen/menus/Co2Menu.hpp"

void Co2Menu::draw(OledScreen &g)
{
	g.DisplayString(2, 54, "CO2:");
	if (values.size() > 0)
	{
		String val(values.back());
		g.DisplayString(85, 54, val.c_str());
		g.DisplayString(85 + val.length() * 6, 54, " ppm");
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
void Co2Menu::onPot(int pot, int value)
{
}
void Co2Menu::onSensorValue(int value)
{
	// Serial.print("Co2 value: ");
	// Serial.println(value);
	values.push_back(value);
	setDirty(true);
}
