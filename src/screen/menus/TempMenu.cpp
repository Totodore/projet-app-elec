#include "screen/menus/TempMenu.hpp"

void TempMenu::draw(OledScreen &g)
{
	g.DisplayString(5, 54, "Temperature:");
	if (values.size() > 0)
	{
		g.DisplayString(105, 54, values.back());
		g.DisplayString(120, 54, "C");
	}
	drawGraph(g, 0, 35);
	for (unsigned short int i = 0; i < values.size(); i++)
	{
		int value = 92 - values[i] / 35.0 * 92;
		if (value > 92)
			value = 92;
		if (value < 0)
			value = 0;
		g.DrawPixel(i, value, false);
		if (i >= 127)
		{
			values.clear();
			g.Reset();
		}
	}
}
void TempMenu::onPot(int pot, int value)
{
}
void TempMenu::onSensorValue(int value)
{
	Serial.print("Temperature value: ");
	Serial.println(value);
	values.push_back(value);
	setDirty(true);
}