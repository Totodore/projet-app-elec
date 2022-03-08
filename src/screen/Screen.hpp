#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <vector>

#include "OledScreen.hpp"
#include "./menus/Home.hpp"
#include "../BaseWorker.cpp"
#include "./Menu.hpp"

class Screen : public BaseWorker
{

public:
	void init()
	{
		Serial.println("Starting screen...");
		screen.InitI2C();
		screen.InitScreen();
		screen.Reset();
		Serial.println("Screen started");
		addMenu(new Home());
	}

	void addMenu(Menu *menu)
	{
		menus.push_back(menu);
	}

	void loop()
	{
		menus[currentMenu]->draw(screen);
	}

	~Screen()
	{
		for (size_t i = 0; i < menus.size(); i++)
		{
			delete menus[i];
		}
	}

private:
	void readPot()
	{
		for (int i = 0; i < 3; i++)
		{
			int val = analogRead(i + 27); // Potentiometer pins are 27, 28 and 29
			if (val > pots[i])
			{
				pots[i] = val;
				dispatchPot(i, val);
			}
		}
	}
	void dispatchPot(short int pot, short int value)
	{
		for (auto menu : menus)
		{
			menu->onPot(pot, value);
		}
	}

private:
	OledScreen screen;
	std::vector<Menu*> menus;
	short int currentMenu = 0;
	short int pots[3] = {0, 0, 0};
};

#endif // SCREEN_HPP