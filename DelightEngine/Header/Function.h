#pragma once

// Global Functions.

namespace Delight
{
	static void memzero(void* ptr, UInt32 size)
	{
		memset(ptr, 0, size);
	}
}