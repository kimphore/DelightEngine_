#include "DX12_VertexBuffer.h"

#include "Include.h"
#include "RHI_DX12Device.h"
#include "DX12_ResourceUploadPool.h"

bool8 CDX12_VertexBuffer::CreateBuffer(CRHIDirectX12* RHI, EBufferType InType, uint32 InSize, void* InData)
{
	if (RHI && InSize > 0)
	{
		ID3D12Device* Device = RHI->GetDevice();
		if (Device)
		{
			Type = InType;
			Size = InSize;

			D3D12_HEAP_PROPERTIES HeapProperty; // ±Û·Î¹ú·Î »¬¼öµµ

			HeapProperty.Type = TranslateHeapType(Type);
			HeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			HeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			HeapProperty.CreationNodeMask = 1;
			HeapProperty.VisibleNodeMask = 1;

			D3D12_RESOURCE_DESC ResourceDesc;

			ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			ResourceDesc.Alignment = 0; // default 64kb
			ResourceDesc.Width = Size;
			ResourceDesc.Height = 1;
			ResourceDesc.DepthOrArraySize = 1;
			ResourceDesc.MipLevels = 0; // auto calc
			ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
			ResourceDesc.SampleDesc.Count = 1;
			ResourceDesc.SampleDesc.Quality = 0;
			ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			Device->CreateCommittedResource(
				&HeapProperty,
				D3D12_HEAP_FLAG_NONE,
				&ResourceDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr, // initial data?
				DELIGHT_IID_PPV_ARGS(&Buffer));

			if (InData)
			{
				SetData(InData, Size);
			}

			return true;
		}
	}

	return false;
}

void CDX12_VertexBuffer::SetData(void* InData, uint64 Size)
{
	if (Type == Dynamic)
	{
		// allocate directly.
	}
	else
	{
		// allocated by upload pool.
	}
}
