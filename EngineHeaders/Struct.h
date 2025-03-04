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

struct Int32_2
{
	int32 x = 0;
	int32 y = 0;

	Int32_2() {}
	Int32_2(int32 inX, int32 inY)
		:x(inX), y(inY)
	{
	}

	Int32_2 operator -(const Int32_2& other) const {
		return Int32_2(x - other.x, y - other.y);
	}
};

struct Int32_3
{
	int32 x = 0;
	int32 y = 0;
	int32 z = 0;

	Int32_3() {}
	Int32_3(int32 inX, int32 inY, int32 inZ)
		:x(inX), y(inY), z(inZ)
	{
	}
};