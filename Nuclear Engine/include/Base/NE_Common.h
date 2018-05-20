#pragma once
#include <Base\Utilities\Logger.h>
#include <Base\BuildConfig.h>
#include <Base\NE_PrivateCommon.h>
#include <Base\NE_Exception.h>

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
//https://stackoverflow.com/questions/523724/c-c-check-if-one-bit-is-set-in-i-e-int-variable
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

namespace NuclearEngine {

	//NEAPI std::wstring String2WSTR(const std::string& s);
}