#include "screen/menus/MicMenu.hpp"

void MicMenu::draw(OledScreen &g)
{
	g.DisplayString(5, 54, "Bruit ambiant:");
	if (values.size() > 0)
	{
		g.DisplayString(105, 54, values.back());
		g.DisplayString(120, 54, "dB");
	}
	drawGraph(g, 0, 120);
	for (unsigned short int i = 0; i < values.size(); i++)
	{
		int value = 92 - values[i] / 120.0 * 92;
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
void MicMenu::onPot(int pot, int value)
{
}
void MicMenu::onSensorValue(int value)
{
	Serial.print("Microphone value: ");
	Serial.println(value);
	values.push_back(value);
	setDirty(true);
}
