#pragma once

#include "Include.h"

#include "d3d12.h"

#include "eastl/string.h"

struct FTextureCreateDesc
{
	int32 SizeX = 1;
	int32 SizeY = 1;
	int32 SizeZ = 1;
	int32 MipLevels = 1;

	DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN;
	D3D12_RESOURCE_FLAGS Flags = D3D12_RESOURCE_FLAG_NONE;

	eastl::string ResourceName = eastl::string();
};

struct FRendertargetCreateDesc :public FTextureCreateDesc
{
	float32 ClearColor[4] = {0.f, 0.f, 0.f, 1.f};
};