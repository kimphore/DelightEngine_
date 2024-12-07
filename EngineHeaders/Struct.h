#pragma once

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