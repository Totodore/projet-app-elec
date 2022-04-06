#pragma once

#include "Screen.hpp"

class Menu
{
public:
	Menu() : dirty(true) {
		Serial.println("New menu");
	}
	virtual void draw(OledScreen &g) = 0;
	virtual void onPot(int pot, int value) = 0;
	virtual void onSensorValue(int value) = 0;
	void setDirty()
	{
		dirty = true;
	}
	void setDirty(bool val)
	{
		dirty = val;
	}
	bool isDirty()
	{
		return dirty;
	}

	virtual ~Menu() {}

protected:
	void drawGraph(OledScreen &g, int min, int max)
	{
		for (int i = 0; i < 128; i++)
			g.DrawPixel(i, 92, false);
		for (int i = 0; i < 92; i++)
			g.DrawPixel(0, i, false);
	}
private:
	bool dirty;
};