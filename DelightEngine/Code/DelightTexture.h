#pragma once

#include <d3d12.h>

#include "Include.h"

#include "eastl/string.h"
#include "DX12_CommandList.h"
#include "comptr.h"

class CDelightTexture
{
public:
	void CreateTexture(CDX12_CommandList& CommandList, eastl::string& InPath, eastl::string& InTextureName);
	void Release();

public:
	void ReleaseUploadBuffer() {
		UploadBuffer.Release();
	};

private:
	Delight::Comptr<ID3D12Resource> Resource;
	Delight::Comptr<ID3D12Resource> UploadBuffer;
	Delight::Comptr<ID3D12Device> Device;

	eastl::string TextureName;
	Delight::FInt32_2 Size;
};