#pragma once

#include "Include.h"
#include "EASTL/string.h"
#include "DX12_DescriptorHeapManager.h"

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
	D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorCPUHandle(ERenderTargetState InType);
	D3D12_GPU_DESCRIPTOR_HANDLE GetDescriptorGPUHandle(ERenderTargetState InType);

public:
	void TransitionToState(CDX12_CommandList* CommandList, ERenderTargetState NextState);

protected:
	inline D3D12_RESOURCE_STATES TranslateResourceState(ERenderTargetState InState);

protected:
	Delight::Comptr<ID3D12Device> Device;
	Delight::Comptr<ID3D12Resource> Resource;
	FDescriptorHandleSet RTVHandle; // RenderTargetView
	FDescriptorHandleSet DSVHandle; // DepthStencilView
	FDescriptorHandleSet SRVHandle; // ShaderResourceView
	FDescriptorHandleSet UAVHandle; // UnorderedAccessView

protected:
	ERenderTargetState CurrentState;
	ERenderTargetType Type;
	DXGI_FORMAT Format;
	uint32 Width;
	uint32 Height;
	TCHAR DebugName[64];
};