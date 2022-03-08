#ifndef MENU_HPP
#define MENU_HPP

#include "Screen.hpp"

class Menu {
	public:
		virtual void draw(OledScreen& g) = 0;
		virtual void onPot(int pot, int value) = 0;

		virtual ~Menu() {}
};
#endif // MENU_HPP