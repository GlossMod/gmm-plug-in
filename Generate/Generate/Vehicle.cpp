#include "Vehicle.h"

// 通过名称获取载具
void generate_vehicle(std::string modelName) 
{
	WAIT(500);
	auto model = GET_HASH_KEY(modelName);

	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model))
	{
		STREAMING::REQUEST_MODEL(model); // 搜索模型
		while (!STREAMING::HAS_MODEL_LOADED(model))
			WAIT(0);																							 // 是否找到模型
		Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0.0); // 获取玩家坐标
		Vehicle newcar = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, 0.0, 1, 1, 0);			 // 创建载具
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(newcar, 0.0f);													 // 将载具正确的放在地面上

		// 生成后进入
		// ENTITY::SET_ENTITY_HEADING(newcar, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
		// PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), newcar, -1);

		ENTITY::SET_ENTITY_COLLISION(newcar, true, true);	// 碰撞
		ENTITY::SET_ENTITY_ALPHA(newcar, 255, false);		// 设置透明度
		VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(newcar, 1);	// 设置强力轴
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(newcar, 0.0f);		// 设置脏污程度
		VEHICLE::SET_VEHICLE_ENVEFF_SCALE(newcar, 0.0f);	// 设置环境效果
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(newcar, 0, 1); // 修复线下 DLC 载具消失的问题

		ENTITY::RESET_ENTITY_ALPHA(newcar);
		VEHICLE::SET_VEHICLE_IS_STOLEN(newcar, false);

		int newnetid = NETWORK::VEH_TO_NET(newcar);
		RequestControlOfId(newnetid);

		WAIT(0);
		if (STREAMING::HAS_MODEL_LOADED(model))
		{
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		}
		// ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&newcar);

		char statusText[32];
		sprintf_s(statusText, "%s 生成完毕.", modelName);
		PrintBottomCentre(statusText);
	}
	else
	{
		PrintBottomCentre("未找到载具");
	}

}

// 通过列表生成载具
class MenuItemList : public MenuItemDefault
{
	string m_modelName;
	virtual void OnSelect()
	{
		generate_vehicle(m_modelName);
	}

public:
	MenuItemList(string caption, string modelName)
		: MenuItemDefault(caption),
		m_modelName(modelName) {}
};

class MenuItemInputModel : public MenuItemDefault
{
	virtual void OnSelect()
	{

		std::string inputStrModel = InputBox("", 28U, "请输入模型名称:", "");

		if (inputStrModel.length() > 0)
		{
			generate_vehicle(inputStrModel);
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

MenuBase* CreateVehicleMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle("载具"));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemInputModel(("输入模型")));

	std::vector<std::string> list = readLinesFromFile("Generate/vehicel.txt");

	for each (std::string item in list)
	{
		menu->AddItem(new MenuItemList(item, item));
	}
	
	return menu;
}