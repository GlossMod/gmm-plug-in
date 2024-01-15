#pragma once
#include "script.h"

#include <string>
#include <sstream>

std::string InputBox(const std::string& escReturn = "", int maxChars = 64U, std::string titlegxt = "", std::string preText = "");
void PrintBottomCentre(std::string s, int time = 2500);
bool RequestControlOfId(int netid);
