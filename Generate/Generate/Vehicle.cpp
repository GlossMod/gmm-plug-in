#include "Vehicle.h"

// ͨ�����ƻ�ȡ�ؾ�
void generate_vehicle(std::string modelName) 
{
	WAIT(500);
	auto model = GET_HASH_KEY(modelName);

	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model))
	{
		STREAMING::REQUEST_MODEL(model); // ����ģ��
		while (!STREAMING::HAS_MODEL_LOADED(model))
			WAIT(0);																							 // �Ƿ��ҵ�ģ��
		Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0.0); // ��ȡ�������
		Vehicle newcar = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, 0.0, 1, 1, 0);			 // �����ؾ�
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(newcar, 0.0f);													 // ���ؾ���ȷ�ķ��ڵ�����

		// ���ɺ����
		// ENTITY::SET_ENTITY_HEADING(newcar, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
		// PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), newcar, -1);

		ENTITY::SET_ENTITY_COLLISION(newcar, true, true);	// ��ײ
		ENTITY::SET_ENTITY_ALPHA(newcar, 255, false);		// ����͸����
		VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(newcar, 1);	// ����ǿ����
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(newcar, 0.0f);		// �������۳̶�
		VEHICLE::SET_VEHICLE_ENVEFF_SCALE(newcar, 0.0f);	// ���û���Ч��
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(newcar, 0, 1); // �޸����� DLC �ؾ���ʧ������

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
		sprintf_s(statusText, "%s �������.", modelName);
		PrintBottomCentre(statusText);
	}
	else
	{
		PrintBottomCentre("δ�ҵ��ؾ�");
	}

}

// ͨ���б������ؾ�
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

		std::string inputStrModel = InputBox("", 28U, "������ģ������:", "");

		if (inputStrModel.length() > 0)
		{
			generate_vehicle(inputStrModel);
		}
		else
		{
			PrintBottomCentre("������Ч");
		}
	}

public:
	MenuItemInputModel(string caption)
		: MenuItemDefault(caption) {}
};

MenuBase* CreateVehicleMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle("�ؾ�"));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemInputModel(("����ģ��")));

	std::vector<std::string> list = readLinesFromFile("Generate/vehicel.txt");

	for each (std::string item in list)
	{
		menu->AddItem(new MenuItemList(item, item));
	}
	
	return menu;
}