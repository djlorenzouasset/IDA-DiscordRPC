#pragma once
#include "../includes.h"

namespace Ida
{
	ea_t GetCursorAddr();
	ea_t GetFunctionAddr();

	const char* GetDBFilename();
	const char* GetFunctionName();
}
