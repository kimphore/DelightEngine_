#pragma once
#include "Typedef.h"

namespace Delight
{
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

struct FInt32_2
{
	int32 x = 0;
	int32 y = 0;

	FInt32_2() {}
	FInt32_2(int32 inX, int32 inY)
		:x(inX), y(inY)
	{
	}
};

struct FInt32_3
{
	int32 x = 0;
	int32 y = 0;
	int32 z = 0;

	FInt32_3() {}
	FInt32_3(int32 inX, int32 inY, int32 inZ)
		:x(inX), y(inY), z(inZ)
	{
	}
};