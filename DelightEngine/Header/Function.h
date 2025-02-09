#pragma once

#include "GlobalVariables.h"
#include "Define.h"
#include "eastl/string.h"

// Global Functions.

namespace Delight
{
	static inline void Memcpy(void* Dest, void* Src, uint64 Size)
	{
		memcpy(Dest, Src, Size);
	}

	static inline void Memzero(void* ptr, uint32 size)
	{
		memset(ptr, 0, size);
	}
	
	static inline void AssertImple(bool8 functionResult, char* assertionText, char* fileName, char* funcName, int cppLine)
	{
		static char assertText[512];

		if (!functionResult)
		{
			Memzero(assertText, 512);

			sprintf_s(assertText, 512,
				"FileName : %s\n\rFunction:%s(%d line)\n\r\n\r%s",
				fileName, funcName, cppLine, assertionText
			);

			__debugbreak();
		}
	}

	#define ASSERT(result, caption) AssertImple(result, caption, __FILE__, __FUNCTION__, __LINE__)

	static bool8 IsInGameThread()
	{
		return GGameThreadID == std::this_thread::get_id();
	}

	static bool8 IsInRenderThread()
	{
		return GRenderThreadID == std::this_thread::get_id();
	}

	static inline void ConvertCharToTChar(eastl::string& InChar, eastl::basic_string<TCHAR>& outTChar)
	{
		int32 Length = MultiByteToWideChar(CP_UTF8, 0, InChar.c_str(), -1, nullptr, 0);
		outTChar = eastl::basic_string<TCHAR>(Length - 1, L'\0');

		MultiByteToWideChar(CP_UTF8, 0, InChar.c_str(), -1, outTChar.data(), Length);
	}
}