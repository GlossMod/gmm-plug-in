#include "Game.h"

void add_text_component_long_string(const std::string& text)
{
	const UINT8 maxStrComponentLength = 99;
	for (int i = 0; i < text.length(); i += maxStrComponentLength)
	{
		const std::string& strComp = text.substr(i, min(text.length() - i, maxStrComponentLength));
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(strComp.c_str());
	}
}

std::string InputBox(const std::string& escReturn, int maxChars, std::string titlegxt, std::string preText)
{
	preText = preText.substr(0, maxChars);

	//CustomKeyboardText ckt;
	MISC::DISPLAY_ONSCREEN_KEYBOARD(true, "", "", preText.c_str(), "", "", "", maxChars);

	while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0)
	{
		HUD::SET_TEXT_FONT(/*GTAfont::Arial*/0);
		HUD::SET_TEXT_SCALE(0.34f, 0.34f);
		HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
		HUD::SET_TEXT_WRAP(0.0f, 1.0f);
		HUD::SET_TEXT_RIGHT_JUSTIFY(FALSE);
		HUD::SET_TEXT_CENTRE(TRUE);
		HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		HUD::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		HUD::SET_TEXT_OUTLINE();

		if (HUD::DOES_TEXT_LABEL_EXIST(titlegxt.c_str()))
		{
			HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT(titlegxt.c_str());
		}
		else
		{
			//titlegxt = Language::TranslateToSelected(titlegxt);
			if (titlegxt.length() < 100)
			{
				HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
				HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(titlegxt.c_str());
			}
			else
			{
				HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("jamyfafi");
				add_text_component_long_string(titlegxt);
			}
		}
		HUD::END_TEXT_COMMAND_DISPLAY_TEXT(0.5f, 0.37f, 0);
		WAIT(0);
	}
	if (MISC::UPDATE_ONSCREEN_KEYBOARD() == 2)
	{
		return escReturn;
	}

	return MISC::GET_ONSCREEN_KEYBOARD_RESULT();
}

void PrintBottomCentre(std::string s, int time)
{
	//s = Language::TranslateToSelected(s);
	const char* text = s.c_str();

	if (HUD::DOES_TEXT_LABEL_EXIST(text))
	{
		HUD::BEGIN_TEXT_COMMAND_PRINT(text);
	}
	else
	{
		if (s.length() < 100)
		{
			HUD::BEGIN_TEXT_COMMAND_PRINT("STRING");
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		}
		else
		{
			HUD::BEGIN_TEXT_COMMAND_PRINT("jamyfafi");
			add_text_component_long_string(s);
		}
	}
	HUD::END_TEXT_COMMAND_PRINT(time, 1);
}

bool RequestControlOfId(int netid)
{
	NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(netid);
	for (DWORD timeOut = GetTickCount() + 200; GetTickCount() < timeOut;)
	{
		if (NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(netid))
			return true;
		WAIT(0);
	}
	return false;
}