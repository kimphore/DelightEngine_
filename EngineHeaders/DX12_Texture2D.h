#pragma once

#include "DX12_Texture.h"

class CDX12_Texture2D : public CDX12_Texture
{
public:
	virtual void CreateTexture(CDX12_CommandList& CommandList, eastl::string& InPath, eastl::string& InTextureName);
};