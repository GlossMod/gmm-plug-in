#include "Player.h"


void change_model(std::string modelName)
{

	auto model = GET_HASH_KEY(modelName);

	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{

		Ped playerPed = PLAYER::PLAYER_PED_ID();
		int oldPlayerPed = playerPed; // 备份原角色

		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			// make_periodic_feature_call();
			WAIT(0);
		}

		Vehicle veh = NULL;
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
		{
			veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
		}


		PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
		PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 0);
		WAIT(0);

		if (veh != NULL)
		{
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
		}


		WAIT(100);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);


		playerPed = PLAYER::PLAYER_PED_ID();

		// 判断是否是动物
		if (PED::GET_PED_TYPE(playerPed) == PedType::Animal && !STREAMING::HAS_ANIM_DICT_LOADED("creatures@rottweiler@melee@streamed_core@")) {
			STREAMING::REQUEST_ANIM_DICT("creatures@rottweiler@melee@streamed_core@");
			STREAMING::REQUEST_ANIM_DICT("creatures@cougar@melee@streamed_core@");
		}
		
	}
}

class MenuItemInputModel : public MenuItemDefault
{
	virtual void OnSelect()
	{

		std::string inputStrModel = InputBox("", 28U, "请输入模型名称:", "");

		if (inputStrModel.length() > 0)
		{
			change_model(inputStrModel);
		}
		else
		{
			PrintBottomCentre("输入无效");
		}
	}

public:
	MenuItemInputModel(string caption)
		: MenuItemDefault(caption) {}
};

MenuBase *CreatePlayerMenu(MenuController *controller)
{
	auto menu = new MenuBase(new MenuItemTitle("人物"));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemInputModel(("输入模型")));

	// menu->AddItem(new MenuItemMenu("MALE YOUNG", CreateHumanSpawnerMenuExactFilter(controller, false, true, false, true, false, false)));
	// menu->AddItem(new MenuItemMenu("MALE MIDDLE", CreateHumanSpawnerMenuExactFilter(controller, false, true, false, false, true, false)));
	// menu->AddItem(new MenuItemMenu("MALE OLD", CreateHumanSpawnerMenuExactFilter(controller, false, true, false, false, false, true)));
	// menu->AddItem(new MenuItemMenu("FEMALE YOUNG", CreateHumanSpawnerMenuExactFilter(controller, false, false, true, true, false, false)));
	// menu->AddItem(new MenuItemMenu("FEMALE MIDDLE", CreateHumanSpawnerMenuExactFilter(controller, false, false, true, false, true, false)));
	// menu->AddItem(new MenuItemMenu("FEMALE OLD", CreateHumanSpawnerMenuExactFilter(controller, false, false, true, false, false, true)));
	// menu->AddItem(new MenuItemMenu("MISC", CreateHumanSpawnerMenuExactFilter(controller, false, false, false, false, false, false)));

	return menu;
}