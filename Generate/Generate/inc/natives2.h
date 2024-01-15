#pragma once

#include "natives.h"

#include "main.h" // scriptWait

#include <string>



Hash GET_HASH_KEY(const char* value);
Hash GET_HASH_KEY(const std::wstring& value);
Hash GET_HASH_KEY(const std::string& value);

