#pragma once

#include "DelightTexture.h"

class CDelightTexture3D :public CDelightTexture
{
public:
	virtual void CreateTexture(CDX12_CommandList& CommandList, eastl::string& InPath, eastl::string& InTextureName);
	virtual void Release();
};