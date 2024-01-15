/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "script.h"

#include "Vehicle.h"
#include "Player.h"
#include "Weapon.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>



MenuBase *CreateMainMenu(MenuController *controller)
{
	auto menu = new MenuBase(new MenuItemTitle("GMM ������"));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMenu("�ؾ�", CreateVehicleMenu(controller)));
	menu->AddItem(new MenuItemMenu("����", CreatePlayerMenu(controller)));
	menu->AddItem(new MenuItemMenu("����", CreateWeaponMenu(controller)));
	return menu;
}

void main()
{
	auto menuController = new MenuController();
	auto mainMenu = CreateMainMenu(menuController);

	while (true)
	{
		if (!menuController->HasActiveMenu() && MenuInput::MenuSwitchPressed())
		{
			MenuInput::MenuInputBeep();
			menuController->PushMenu(mainMenu);
		}
		menuController->Update();
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}