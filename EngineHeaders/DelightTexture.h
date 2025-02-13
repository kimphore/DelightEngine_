#pragma once

#include <d3d12.h>

#include "Include.h"

#include "eastl/string.h"
#include "DX12_CommandList.h"
#include "comptr.h"

class CDX12_Texture;
class CDelightTexture
{
public:
	virtual void CreateTexture(CDX12_CommandList& CommandList, eastl::string& InPath, eastl::string& InTextureName);
	virtual void Release();

public:
	CDX12_Texture* GetTexture() {
		return Texture;
	}
protected:
	CDX12_Texture* Texture;
};