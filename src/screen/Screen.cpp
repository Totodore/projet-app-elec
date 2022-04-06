#include "screen/Screen.hpp"

Screen::~Screen()
{
	for (size_t i = 0; i < menus.size(); i++)
	{
		delete menus[i];
	}
}

void Screen::init()
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

void Screen::addMenu(Menu *menu)
{
	menus.push_back(menu);
}

void Screen::loop()
{
	readPot();
	if (temp->hasNewValue() && currentMenu == 1)
		menus[currentMenu]->onSensorValue(temp->getNewValue());
	else if (temp->hasNewHumidityValue() && currentMenu == 2)
		menus[currentMenu]->onSensorValue(temp->getNewHumidityValue());
	else if (co2->hasNewValue() && currentMenu == 3)
	{
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

void Screen::initPins()
{
	// Pin for potentiometers
	for (int i = 0; i < 3; i++)
	{
		pinMode(i + 27, INPUT);
	}
}
void Screen::readPot()
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
bool Screen::checkPot(short int pot, short int value)
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
void Screen::dispatchPot(short int pot, short int value)
{
	for (auto &menu : menus)
	{
		menu->onPot(pot, value);
	}
}
void Screen::drawScreenPosition(OledScreen &screen)
{
	int screenPart = 127 / menus.size();
	for (int i = 0; i <= currentMenu; i++)
		for (int j = i * screenPart; j < i * screenPart + screenPart - 5; j++)
			screen.DrawPixel(j, 127, false);
}
void Screen::changeMenu(uint8_t menu)
{
	if (menu < 0 || menu >= menus.size())
		return;
	currentMenu = menu;
	menuChanged = true;
}