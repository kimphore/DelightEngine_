#pragma once

// Global Functions.

namespace Delight
{
	static inline void memzero(void* ptr, uint32 size)
	{
		memset(ptr, 0, size);
	}
	
	static inline void assertImple(bool8 functionResult, char* assertionText, char* fileName, char* funcName, int cppLine)
	{
		static char assertText[512];

		if (!functionResult)
		{
			memzero(assertText, 512);

			sprintf_s(assertText, 512,
				"FileName : %s\n\rFunction:%s(%d line)\n\r\n\r%s",
				fileName, funcName, cppLine, assertionText
			);

			__debugbreak();
		}
	}

	#define ASSERT(result, caption) assertImple(result, caption, __FILE__, __FUNCTION__, __LINE__)
}