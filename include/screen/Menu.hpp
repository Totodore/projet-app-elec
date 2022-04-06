#pragma once

#include "Arduino.h"
#include "OledScreen.hpp"
class Menu
{
public:
	Menu() : dirty(true)
	{
		Serial.println("New menu");
	}
	virtual void draw(OledScreen &g) = 0;
	virtual void onPot(int pot, int value) = 0;
	virtual void onSensorValue(int value) = 0;
	void setDirty();
	void setDirty(bool val);
	bool isDirty();

	virtual ~Menu() {}

protected:
	void drawGraph(OledScreen &g, int min, int max);

private:
	bool dirty;
};