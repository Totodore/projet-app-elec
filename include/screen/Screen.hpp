#pragma once

#include <vector>

#include "OledScreen.hpp"

#include "screen/menus/Home.hpp"
#include "screen/menus/TempMenu.hpp"
#include "screen/menus/Co2Menu.hpp"
#include "screen/menus/MicMenu.hpp"
#include "screen/menus/HumMenu.hpp"
#include "screen/Menu.hpp"

#include "Bluetooth.hpp"
#include "sensors/Mic.hpp"
#include "sensors/Temperature.hpp"
#include "sensors/Co2.hpp"

#include "BaseWorker.hpp"

class Screen : public BaseWorker
{

public:
	Screen(Temperature *temp, Co2 *co2, Mic *mic, Bluetooth *ble) : bluetooth(ble), mic(mic), temp(temp), co2(co2) {}
	void init() override;
	void addMenu(Menu *menu);
	void loop() override;
	~Screen();

private:
	void initPins();
	void readPot();
	bool checkPot(short int pot, short int value);
	void dispatchPot(short int pot, short int value);
	void drawScreenPosition(OledScreen &screen);
	void changeMenu(uint8_t menu);

private:
	OledScreen screen;
	std::vector<Menu *> menus;
	short int currentMenu = 0;
	bool menuChanged = false;
	short int pots[3] = {0, 0, 0};

	Bluetooth *bluetooth;
	Mic *mic;
	Temperature *temp;
	Co2 *co2;

	static constexpr short int detectingOffset = 100;
};
