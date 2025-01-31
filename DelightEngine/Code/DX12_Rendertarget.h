#pragma once

#include "Include.h"
#include "EASTL/string.h"
#include "DX12_DescriptorHeapManager.h"

enum ERenderTargetType
{
	Color,
	Depth,
};

enum EResourceState
{
	RS_PRESENT,
	RS_RTV,
	RS_DSV,
	RS_SRV,
	RS_SRV_PS,
	RS_SRV_NonPS,
	RS_SRV_ALL,
	RS_UAV,
};

enum EResourceType
{
	RT_RTV,
	RT_DSV,
	RT_SRV,
	RT_UAV,
};

class CDX12_CommandList;
class CRHIDirectX12;
class CDX12_Rendertarget
{
public:
	void Initialize(CRHIDirectX12* RHI, Delight::Comptr<ID3D12Resource> InResource);
	void Initialize(CRHIDirectX12* RHI, ERenderTargetType InType, uint64 InWidth, uint64 InHeight, DXGI_FORMAT InFormat);
	void CreateRTV();
	void CreateDSV();
	// need to use
	void CreateSRV();
	void CreateUAV();
	void SetDebugName(TCHAR* InDebugName);

public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorCPUHandle(EResourceType InType);
	D3D12_GPU_DESCRIPTOR_HANDLE GetDescriptorGPUHandle(EResourceType InType);

public:
	void TransitionToState(CDX12_CommandList* CommandList, EResourceState InState);

protected:
	inline D3D12_RESOURCE_STATES TranslateResourceState(EResourceState InState);

protected:
	Delight::Comptr<ID3D12Device> Device;
	Delight::Comptr<ID3D12Resource> Resource;
	FDescriptorHandleSet RTVHandle; // RenderTargetView
	FDescriptorHandleSet DSVHandle; // DepthStencilView
	FDescriptorHandleSet SRVHandle; // ShaderResourceView
	FDescriptorHandleSet UAVHandle; // UnorderedAccessView

protected:
	EResourceState CurrentState;
	ERenderTargetType Type;
	DXGI_FORMAT Format;
	uint64 Width;
	uint64 Height;
	TCHAR DebugName[64];
};