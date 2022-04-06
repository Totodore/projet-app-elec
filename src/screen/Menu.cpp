#include "screen/Menu.hpp"

void Menu::setDirty()
{
	dirty = true;
}
void Menu::setDirty(bool val)
{
	dirty = val;
}
bool Menu::isDirty()
{
	return dirty;
}

void Menu::drawGraph(OledScreen &g, int min, int max)
{
	for (int i = 0; i < 128; i++)
		g.DrawPixel(i, 92, false);
	for (int i = 0; i < 92; i++)
		g.DrawPixel(0, i, false);
}