#include "DX12_Rendertarget.h"
#include "RHI_DX12Device.h"
#include "DX12_Functions.h"

// 외부에서 이미 만들어진 Target을 대상으로 세팅.
// 백버퍼만 쓸꺼고, 초기상태는 Present라고함..
void CDX12_Rendertarget::Initialize(Delight::Comptr<ID3D12Device> InDevice, Delight::Comptr<ID3D12Resource> InResource)
{
	Device = InDevice;

	if (Device.IsValid() && InResource.IsValid())
	{
		D3D12_RESOURCE_DESC Desc = InResource->GetDesc();
		bool8 bIsDepthStencil = (Desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) > 0;

		Format = Desc.Format;
		Width = Desc.Width;
		Height = Desc.Height;
		Type = bIsDepthStencil ? Depth : Color;
		SetResourceState(D3D12_RESOURCE_STATE_PRESENT);
		Resource = InResource;

		if (bIsDepthStencil)
		{
			CreateDSV();
		}
		else
		{
			CreateRTV();
		}
	}
}

void CDX12_Rendertarget::CreateRendertarget(Delight::Comptr<ID3D12Device> InDevice, ERenderTargetType InType, FRendertargetCreateDesc& InDesc)
{
	Device = InDevice;
	if (Device.IsValid())
	{
		bool8 bIsDepthStencil = Type == Depth;

		Format = InDesc.Format;
		Size.x = InDesc.SizeX;
		Size.y = InDesc.SizeY;
		Size.z = InDesc.SizeZ;
		Type = InType;

		// 쉐이더에서 읽을땐 DEPTH_READ로
		SetResourceState(bIsDepthStencil ? D3D12_RESOURCE_STATE_DEPTH_WRITE : D3D12_RESOURCE_STATE_RENDER_TARGET);

		D3D12_HEAP_PROPERTIES HeapProperty;
		HeapProperty.Type = D3D12_HEAP_TYPE_DEFAULT;
		HeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		HeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		HeapProperty.CreationNodeMask = 1;
		HeapProperty.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC ResourceDesc;
		ResourceDesc.Dimension = Size.z > 1 ? D3D12_RESOURCE_DIMENSION_TEXTURE3D : D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		ResourceDesc.Alignment = 0;
		ResourceDesc.Width = Size.x;
		ResourceDesc.Height = Size.y;
		ResourceDesc.DepthOrArraySize = Size.z;
		ResourceDesc.MipLevels = InDesc.MipLevels;
		ResourceDesc.Format = Format; 
		ResourceDesc.SampleDesc.Count = 1;
		ResourceDesc.SampleDesc.Quality = 0;
		ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		ResourceDesc.Flags = InDesc.Flags; // 외부에서 세팅 다하는걸로..

		D3D12_CLEAR_VALUE ClearColor = { Format, InDesc.ClearColor[0], InDesc.ClearColor[1],
			InDesc.ClearColor[2], InDesc.ClearColor[3] };

		Delight::ThrowIfFailed(Device->CreateCommittedResource(
			&HeapProperty,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			GetResourceState(),
			&ClearColor,
			DELIGHT_IID_PPV_ARGS(&Resource)));

		if (bIsDepthStencil)
		{
			CreateDSV();
		}
		else
		{
			CreateRTV();
		}
	}
}

void CDX12_Rendertarget::CreateRTV()
{
	D3D12_RENDER_TARGET_VIEW_DESC RTVDesc = {};
	RTVDesc.Format = Format;
	RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;

	extern CDX12_DescriptorHeapManager GDescriptorHeapManager;
	RTVHandle = GDescriptorHeapManager.GetHandle(DHT_RTV);

	Device->CreateRenderTargetView(Resource.GetData(), &RTVDesc, GetDescriptorCPUHandle(RT_RTV));
}

void CDX12_Rendertarget::CreateDSV()
{
	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
	DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;

	extern CDX12_DescriptorHeapManager GDescriptorHeapManager;
	DSVHandle = GDescriptorHeapManager.GetHandle(DHT_DSV);

	Device->CreateDepthStencilView(Resource.GetData(), &DSVDesc, GetDescriptorCPUHandle(RT_DSV));
}

void CDX12_Rendertarget::CreateSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = Format;
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SRVDesc.Texture2D.MipLevels = 1;

	extern CDX12_DescriptorHeapManager GDescriptorHeapManager;
	SRVHandle = GDescriptorHeapManager.GetHandle(DHT_ShaderResource);

	Device->CreateShaderResourceView(Resource.GetData(), &SRVDesc, GetDescriptorCPUHandle(RT_SRV));
}

void CDX12_Rendertarget::CreateUAV()
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
	UAVDesc.Format = Format;
	UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	UAVDesc.Texture2D.MipSlice = 0;

	extern CDX12_DescriptorHeapManager GDescriptorHeapManager;
	UAVHandle = GDescriptorHeapManager.GetHandle(DHT_ShaderResource);

	Device->CreateUnorderedAccessView(Resource.GetData(), nullptr, &UAVDesc, GetDescriptorCPUHandle(RT_UAV));
}

void CDX12_Rendertarget::SetDebugName(TCHAR* InDebugName)
{
	if (DebugName)
	{
		size_t Length = std::min<size_t>(_tcslen(InDebugName), 64);

		Delight::Memcpy(DebugName, InDebugName, (Length + 1) * 2);
		
		if (Resource.IsValid())
		{
			Resource->SetName(DebugName);
		}
	}
}

D3D12_CPU_DESCRIPTOR_HANDLE CDX12_Rendertarget::GetDescriptorCPUHandle(EResourceType InType)
{
	switch (InType)
	{
	case RT_RTV:
		return RTVHandle.CPUHandle;
	case RT_DSV:
		return DSVHandle.CPUHandle;
	case RT_SRV:
		return SRVHandle.CPUHandle;
	case RT_UAV:
		return UAVHandle.CPUHandle;
	}

	return D3D12_CPU_DESCRIPTOR_HANDLE();
}

D3D12_GPU_DESCRIPTOR_HANDLE CDX12_Rendertarget::GetDescriptorGPUHandle(EResourceType InType)
{
	switch (InType)
	{
	case RT_RTV:
		return RTVHandle.GPUHandle;
	case RT_DSV:
		return DSVHandle.GPUHandle;
	case RT_SRV:
		return SRVHandle.GPUHandle;
	case RT_UAV:
		return UAVHandle.GPUHandle;
	}

	return D3D12_GPU_DESCRIPTOR_HANDLE();
}