#pragma once

struct FMD5Hash
{
	Byte hash[128];

	UInt32 GetSize() {
		return 128;
	}

	FMD5Hash()
	{
		memset(hash, 0, sizeof(Byte) * 128);
	}

};