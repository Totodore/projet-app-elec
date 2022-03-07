#include "./screen/OledScreen.hpp"
#include "../data/logo.h"
#include "BaseWorker.cpp"

class Screen : public BaseWorker
{
	
public:
	void init()
	{
		Serial.println("Starting screen...");
		screen.InitI2C();
		screen.InitScreen();
		screen.Reset();
		screen.Display(logo);
		screen.DisplayString(30, 71, "MyEcovillage");
		Serial.println("Screen started");
	}

	void loop()
	{
	}
private:
	OledScreen screen;
};