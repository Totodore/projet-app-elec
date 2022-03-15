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

private:
	bool dirty;
};