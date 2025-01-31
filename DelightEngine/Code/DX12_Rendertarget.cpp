#include "DX12_Rendertarget.h"
#include "RHI_DX12Device.h"
#include "DX12_Functions.h"

void CDX12_Rendertarget::Initialize(CRHIDirectX12* RHI, ERenderTargetType InType, uint32 InWidth, uint32 InHeight, DXGI_FORMAT InFormat)
{
	if (RHI)
	{
		Device = RHI->GetDevice();
		if (Device.IsValid())
		{
			bool8 bDepthStencil = Type == Depth;

			Format = InFormat;
			Width = InWidth;
			Height = InHeight;
			Type = InType;
			CurrentState = bDepthStencil ? DSV : RTV;

			D3D12_HEAP_PROPERTIES HeapProperty;
			HeapProperty.Type = D3D12_HEAP_TYPE_DEFAULT;
			HeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			HeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			HeapProperty.CreationNodeMask = 1;
			HeapProperty.VisibleNodeMask = 1;

			D3D12_RESOURCE_DESC ResourceDesc;
			ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			ResourceDesc.Alignment = 0;
			ResourceDesc.Width = Width;
			ResourceDesc.Height = Height;
			ResourceDesc.DepthOrArraySize = 1;
			ResourceDesc.MipLevels = 1;
			ResourceDesc.Format = Format;
			ResourceDesc.SampleDesc.Count = 1;
			ResourceDesc.SampleDesc.Quality = 0;
			ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			ResourceDesc.Flags = bDepthStencil ? D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL : D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			ResourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

			D3D12_CLEAR_VALUE ClearColor;
			ClearColor.Format = Format;
			ClearColor.Color[0] = 0.f;
			ClearColor.Color[1] = 0.f;
			ClearColor.Color[2] = 0.f;
			ClearColor.Color[3] = 1.f;

			Delight::ThrowIfFailed(Device->CreateCommittedResource(
				&HeapProperty,
				D3D12_HEAP_FLAG_NONE,
				&ResourceDesc,
				TranslateResourceState(CurrentState),
				&ClearColor,
				DELIGHT_IID_PPV_ARGS(&Resource)));

			if (bDepthStencil)
			{
				CreateDSV();
			}
			else
			{
				CreateRTV();
			}
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
	RTVHandle = GDescriptorHeapManager.GetHandle(EDescriptorHeapType::RTV);

	Device->CreateRenderTargetView(Resource.GetData(), &RTVDesc, GetDescriptorCPUHandle(ERenderTargetState::RTV));
}

void CDX12_Rendertarget::CreateDSV()
{
	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
	DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;

	extern CDX12_DescriptorHeapManager GDescriptorHeapManager;
	DSVHandle = GDescriptorHeapManager.GetHandle(EDescriptorHeapType::DSV);

	Device->CreateDepthStencilView(Resource.GetData(), &DSVDesc, GetDescriptorCPUHandle(ERenderTargetState::DSV));
}

void CDX12_Rendertarget::CreateSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = Format;
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SRVDesc.Texture2D.MipLevels = 1;

	extern CDX12_DescriptorHeapManager GDescriptorHeapManager;
	SRVHandle = GDescriptorHeapManager.GetHandle(EDescriptorHeapType::ShaderResource);

	Device->CreateShaderResourceView(Resource.GetData(), &SRVDesc, GetDescriptorCPUHandle(ERenderTargetState::SRV_PS));
}

void CDX12_Rendertarget::CreateUAV()
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
	UAVDesc.Format = Format;
	UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	UAVDesc.Texture2D.MipSlice = 0;

	extern CDX12_DescriptorHeapManager GDescriptorHeapManager;
	UAVHandle = GDescriptorHeapManager.GetHandle(EDescriptorHeapType::ShaderResource);

	Device->CreateUnorderedAccessView(Resource.GetData(), nullptr, &UAVDesc, GetDescriptorCPUHandle(ERenderTargetState::UAV));
}

void CDX12_Rendertarget::SetDebugName(TCHAR* InDebugName)
{
	if (DebugName)
	{
		uint32 Length = std::min<size_t>(_tcslen(InDebugName), 64);

		Delight::Memcpy(DebugName, InDebugName, (Length + 1) * 2);
		
		if (Resource.IsValid())
		{
			Resource->SetName(DebugName);
		}
	}
}

D3D12_CPU_DESCRIPTOR_HANDLE CDX12_Rendertarget::GetDescriptorCPUHandle(ERenderTargetState InType)
{
	switch (InType)
	{
	case RTV:
		return RTVHandle.CPUHandle;
	case DSV:
		return DSVHandle.CPUHandle;
	case SRV_PS:
	case SRV_NonPS:
	case SRV_ALL:
		return SRVHandle.CPUHandle;
	case UAV:
		return UAVHandle.CPUHandle;
	}
}

D3D12_GPU_DESCRIPTOR_HANDLE CDX12_Rendertarget::GetDescriptorGPUHandle(ERenderTargetState InType)
{
	switch (InType)
	{
	case RTV:
		return RTVHandle.GPUHandle;
	case DSV:
		return DSVHandle.GPUHandle;
	case SRV_PS:
	case SRV_NonPS:
	case SRV_ALL:
		return SRVHandle.GPUHandle;
	case UAV:
		return UAVHandle.GPUHandle;
	}
}

void CDX12_Rendertarget::TransitionToState(CDX12_CommandList* CommandList, ERenderTargetState NextState)
{
	D3D12_RESOURCE_BARRIER ResourceBarrier = Delight::DX12::GetTransitionBarrier(Resource.GetData()
		, TranslateResourceState(CurrentState), TranslateResourceState(NextState));

	CommandList->Get()->ResourceBarrier(1, &ResourceBarrier);
	CurrentState = NextState;
}

D3D12_RESOURCE_STATES CDX12_Rendertarget::TranslateResourceState(ERenderTargetState InState)
{
	switch (InState)
	{
	case RTV:
		return D3D12_RESOURCE_STATE_RENDER_TARGET;
	case DSV:
		return D3D12_RESOURCE_STATE_DEPTH_WRITE;
	case SRV_PS:
		return D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	case SRV_NonPS:
		return D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	case SRV_ALL:
		return D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE;
	case UAV:
		return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	}
}
