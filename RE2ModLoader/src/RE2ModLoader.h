#pragma once
#include "dllmain.hpp"

std::string wideCharToMultiByte(wchar_t *pWCStrKey);
void multiByteToWideChar(const std::string &pKey, wchar_t *pWCStrKey);