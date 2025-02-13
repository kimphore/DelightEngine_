#pragma once

#include "Include.h"
#include "DX12_Resource.h"
#include "EASTL/string.h"
#include "DX12_DescriptorHeapManager.h"
#include "DX12_Header.h"

enum ERenderTargetType
{
	Color,
	Depth,
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
class CDX12_Rendertarget : public CDX12_Resource
{
public:
	void Initialize(Delight::Comptr<ID3D12Device> InDevice, Delight::Comptr<ID3D12Resource> InResource);
	void CreateRendertarget(Delight::Comptr<ID3D12Device> InDevice, ERenderTargetType InType, FRendertargetCreateDesc& InDesc);
	void CreateRTV();
	void CreateDSV();
	// need to use
	void CreateSRV();
	void CreateUAV();
	void SetDebugName(TCHAR* InDebugName);

public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorCPUHandle(EResourceType InType);
	D3D12_GPU_DESCRIPTOR_HANDLE GetDescriptorGPUHandle(EResourceType InType);

protected:
	FDescriptorHandleSet RTVHandle; // RenderTargetView
	FDescriptorHandleSet DSVHandle; // DepthStencilView
	FDescriptorHandleSet SRVHandle; // ShaderResourceView
	FDescriptorHandleSet UAVHandle; // UnorderedAccessView

protected:
	ERenderTargetType Type;
	DXGI_FORMAT Format;
	Delight::FInt32_3 Size;
	uint64 Width;
	uint64 Height;
	TCHAR DebugName[64];
};