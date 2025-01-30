#pragma once

#include "Include.h"
#include "EASTL/string.h"

enum ERenderTargetType
{
	Color,
	Depth,
};

enum ERenderTargetState
{
	RTV,
	DSV,
	SRV_PS,
	SRV_NonPS,
	SRV_ALL,
	UAV,
};

class CDX12_CommandList;
class CRHIDirectX12;
class CDX12_Rendertarget
{
public:
	void Initialize(CRHIDirectX12* RHI, ERenderTargetType InType, uint32 InWidth, uint32 InHeight, DXGI_FORMAT InFormat);
	void CreateRTV();
	void CreateDSV();
	// need to use
	void CreateSRV();
	void CreateUAV();
	void SetDebugName(TCHAR* InDebugName);

public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle(ERenderTargetState InType);

public:
	void TransitionToState(CDX12_CommandList* CommandList, ERenderTargetState NextState);

protected:
	inline D3D12_RESOURCE_STATES TranslateResourceState(ERenderTargetState InState);

protected:
	Delight::Comptr<ID3D12Device> Device;
	Delight::Comptr<ID3D12Resource> Resource;
	D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle; // RenderTargetView
	D3D12_CPU_DESCRIPTOR_HANDLE DSVHandle; // DepthStencilView
	D3D12_CPU_DESCRIPTOR_HANDLE SRVHandle; // ShaderResourceView
	D3D12_CPU_DESCRIPTOR_HANDLE UAVHandle; // UnorderedAccessView

protected:
	ERenderTargetState CurrentState;
	ERenderTargetType Type;
	DXGI_FORMAT Format;
	uint32 Width;
	uint32 Height;
	TCHAR DebugName[64];
};