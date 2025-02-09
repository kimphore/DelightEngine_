#pragma once
#include "Typedef.h"

namespace Delight
{
	struct FInt32_2
	{
		int32 x = 0;
		int32 y = 0;

		FInt32_2() {}
		FInt32_2(int32 inX, int32 inY)
			:x(inX), y(inY)
		{}
	};

	struct FMD5Hash
	{
		byte hash[128];

		uint32 GetSize() {
			return 128;
		}

		FMD5Hash()
		{
			memset(hash, 0, sizeof(byte) * 128);
		}
	};
}