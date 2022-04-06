#include "screen/menus/Home.hpp"

void Home::draw(OledScreen &g)
{
	g.Display(logo);
	g.DisplayString(30, 54, "MyEcovillage");
}
void Home::onPot(int pot, int value)
{
}
void Home::onSensorValue(int value)
{
}
