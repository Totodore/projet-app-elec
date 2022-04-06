#pragma once

#include <cstdarg>
#include <cstdbool>
#include <cstdint>
#include <cmath>

#include <Arduino.h>
#include <tgmath.h>

#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

#include "data/fontData.hpp"

#define PORTI2C 2 // Selection du port I2C 0, 1 ou 2

#define GPIO_PB2_I2C0SCL 0x00010803
#define GPIO_PB3_I2C0SDA 0x00010C03
#define GPIO_PA6_I2C1SCL 0x00001803
#define GPIO_PA7_I2C1SDA 0x00001C03
#define GPIO_PE4_I2C2SCL 0x00041003
#define GPIO_PE5_I2C2SDA 0x00041403

#define LCD_SLAVE_ADDR 0x3C // L'adresse semble être 0x3C
#define LCD_ON 0x4f
#define LCD_OFF 0x4e
#define PUMP 0x14 // 0x8d

#define NORMALDISPLAY 0xA6
#define INVERTDISPLAY 0xA7
#define ACTIVATESCROLL 0x2F
#define DEACTIVATESCROLL 0x2E
#define SETVERTICALSCROLLAREA 0xA3
#define RIGHTHORIZONTALSCROLL 0x26
#define LEFT_HORIZONTALSCROLL 0x27
#define VERTICALRIGHTHORIZONTALSCROLL 0x29
#define VERTICALLEFTHORIZONTALSCROLL 0x2A

#define SX 16
#define EX 96

#define SY 3
#define EY 5

class OledScreen
{

public:
	// initialize I2C module 0
	// Slightly modified version of TI's example code
	void InitI2C0(void);

	void InitI2C1(void);

	void InitI2C2(void);

	void InitI2C(void);
	void I2CSend(uint8_t slave_addr, uint8_t num_of_args, ...);

	void I2C2Send(uint8_t slave_addr, uint8_t num_of_args, ...);

	void mydelay(int duration);

	void WriteCmd(int cmd);
	void WriteData(int data);

	void SetPageAddress(uint8_t add);
	void SetColumnAddress(uint8_t add);

	void Fill(int val);

	void InvertDisplay(int inv);

	void Display(const unsigned char *p);
	void DrawPixel(const uint8_t x, const uint8_t y, bool clear);
	void DrawPixel(const uint8_t x, const uint8_t y);
	void Reset();

	void InitScreen(void);

	void DisplayCarac(int x, int y, char c);

	void DisplayString(int x, int y, const char *s);

	void DisplayString(int x, int y, int number);
	void DisplayStringNextVoid(int x, int y, int number);

private:
	char hexToAscii(char digH);
	uint8_t pixelData[128][8] = {};
};