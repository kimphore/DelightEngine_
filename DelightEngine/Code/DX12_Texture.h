#pragma once
#include "Include.h"

#include "d3d12.h"

#include "DX12_Resource.h"
#include "DX12_Header.h"

class CDX12_Texture : public CDX12_Resource
{
public:
	// create from texture
	virtual void CreateTexture(CDX12_CommandList& CommandList, eastl::string& InPath, eastl::string& InTextureName) = 0;
	virtual void CreateTexture(CDX12_CommandList& CommandList, FTextureCreateDesc& InDesc);
public:
	virtual void DeleteUploadBuffer();
protected:
	Delight::Comptr<ID3D12Resource> UploadBuffer;
	Delight::FInt32_3 Size;
};