#pragma once

#include "script.h"

MenuBase* CreatePlayerMenu(MenuController* controller);



enum PedType {
	Player = 1,
	Male = 4,
	Female = 5,
	Cop = 6,
	Human = 26,
	SWAT = 27,
	Animal = 28,
	Army = 29
};