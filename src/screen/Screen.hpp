#pragma once

#include <vector>

#include "OledScreen.hpp"

#include "./Menu.hpp"
#include "./menus/Home.hpp"
#include "./menus/TempMenu.hpp"
#include "./menus/Co2Menu.hpp"
#include "./menus/MicMenu.hpp"
#include "./menus/HumMenu.hpp"

#include "./Bluetooth.hpp"
#include "./sensors/Mic.hpp"
#include "./sensors/Temperature.hpp"
#include "./sensors/Co2.hpp"

#include "../BaseWorker.hpp"

class Screen : public BaseWorker
{

public:
	Screen(Temperature *temp, Co2 *co2, Mic *mic, Bluetooth *ble) : bluetooth(ble), mic(mic), temp(temp), co2(co2)  {}
	void init() override
	{
		Serial.println("Starting screen...");
		screen.InitI2C();
		screen.InitScreen();
		screen.Reset();
		initPins();
		Serial.println("Screen started");
		addMenu(new Home());
		addMenu(new TempMenu());
		addMenu(new HumMenu());
		addMenu(new Co2Menu());
		addMenu(new MicMenu());
	}

	void addMenu(Menu *menu)
	{
		menus.push_back(menu);
	}

	void loop() override
	{
		readPot();
		if (temp->hasNewValue() && currentMenu == 1)
			menus[currentMenu]->onSensorValue(temp->getNewValue());
		else if (temp->hasNewHumidityValue() && currentMenu == 2)
			menus[currentMenu]->onSensorValue(temp->getNewHumidityValue());
		else if (co2->hasNewValue() && currentMenu == 3) {
			menus[currentMenu]->onSensorValue(co2->getNewValue());
		}
		if (menuChanged)
		{
			menuChanged = false;
			screen.Reset();
			menus[currentMenu]->setDirty(true);
		}
		if (menus[currentMenu]->isDirty())
		{
			menus[currentMenu]->draw(screen);
			menus[currentMenu]->setDirty(false);
			drawScreenPosition(screen);
		}
	}

	~Screen()
	{
		for (size_t i = 0; i < menus.size(); i++)
		{
			delete menus[i];
		}
	}

private:
	void initPins()
	{
		// Pin for potentiometers
		for (int i = 0; i < 3; i++)
		{
			pinMode(i + 27, INPUT);
		}
	}
	void readPot()
	{
		for (int i = 0; i < 3; i++)
		{
			int val = analogRead(i + 27); // Potentiometer pins are 27, 28 and 29
			if (val > pots[i] + detectingOffset || val < pots[i] - detectingOffset)
			{
				pots[i] = val;
				if (checkPot(i, val))
					dispatchPot(i, val);
			}
		}
	}
	bool checkPot(short int pot, short int value)
	{
		if (pot == 1)
		{
			short int menuIndex = floor((value / 4095.0) * (int)menus.size());
			if (menuIndex != currentMenu)
				changeMenu(menuIndex);
			return false;
		}
		return true;
	}
	void dispatchPot(short int pot, short int value)
	{
		for (auto &menu : menus)
		{
			menu->onPot(pot, value);
		}
	}
	void drawScreenPosition(OledScreen &screen)
	{
		int screenPart = 127 / menus.size();
		for (int i = 0; i <= currentMenu; i++)
			for (int j = i * screenPart; j < i * screenPart + screenPart - 5; j++)
				screen.DrawPixel(j, 127, false);
	}
	void changeMenu(uint8_t menu)
	{
		if (menu < 0 || menu >= menus.size())
			return;
		currentMenu = menu;
		menuChanged = true;
	}

private:
	OledScreen screen;
	std::vector<Menu *> menus;
	short int currentMenu = 0;
	bool menuChanged = false;
	short int pots[3] = {0, 0, 0};
	static constexpr short int detectingOffset = 100;

	Bluetooth *bluetooth;
	Mic *mic;
	Temperature *temp;
	Co2 *co2;
};
