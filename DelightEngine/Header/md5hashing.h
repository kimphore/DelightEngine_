#pragma once

#include "include.h"
#include <iostream>
#include <windows.h>
#include <wincrypt.h>

/*!
 * \file md5hashing.h
 * \date 2019/08/02 13:02
 *
 * \author kimphore
 * Contact: user@company.com
 *
 * \brief make md5 hash. for checksum.
 *        from https://rosagigantea.tistory.com/415
 *
 * TODO: long description
 *
 * \note
*/

Bool8 GetMD5Hash(Char* buffer, UInt32 bufferSize, Delight::FMD5Hash* hashOutput)
{
	UInt32 dwState = 0;
	Bool8 bResult = false;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	Dword dwHashSize = 0;
		
	try
	{
		if (0 == CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
			throw "CryptAcquireContext";

		if (0 == CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
			throw "CryptCreateHash";

		if (0 == CryptHashData(hHash, (const Byte*)buffer, bufferSize, 0))
			throw "CryptHashData";

		Dword dwCount = sizeof(Dword);
		if (0 == CryptGetHashParam(hHash, HP_HASHSIZE, (Byte*)&dwHashSize, &dwCount, 0))
			throw "CryptGetHashParam";

		if (hashOutput->GetSize() < dwHashSize)
		{
			bResult = TRUE;
			throw "Memory problem";
		}
		
		Dword hashSize;
		if (CryptGetHashParam(hHash, HP_HASHVAL, hashOutput->hash, &hashSize, 0))
		{
			bResult = TRUE;
		}
		else
		{
			throw "CryptGetHashParam";
		}

	}
	catch (Char* errMsg)
	{
		log(errMsg);
	}

	if (hHash) CryptDestroyHash(hHash);
	if (hProv) CryptReleaseContext(hProv, 0);

	return bResult;
}