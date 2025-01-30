#include "DX12_VertexBuffer.h"

#include "Include.h"
#include "RHI_DX12Device.h"
#include "DX12_ResourceUploadPool.h"

bool8 CDX12_VertexBuffer::CreateBuffer(CRHIDirectX12* RHI, CDX12_CommandList* CommandList, EBufferType InType, EVertexType InVertexType, uint32 InSize, void* InData)
{
	if (RHI && InSize > 0)
	{
		Device = RHI->GetDevice();
		if (Device.IsValid())
		{
			Type = InType;
			Size = InSize;
			VertexType = InVertexType;

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
				SetData(RHI, CommandList, InData, Size);
			}

			VertexBufferView.BufferLocation = Buffer->GetGPUVirtualAddress();
			VertexBufferView.StrideInBytes = GetStrideSize(VertexType);
			VertexBufferView.SizeInBytes = Size;

			return true;
		}
	}

	return false;
}

void CDX12_VertexBuffer::SetData(CRHIDirectX12* RHI, CDX12_CommandList* CommandList, void* InData, uint64 Size)
{
	if (!Buffer.IsValid() || InData == nullptr || Size <= 0 || CommandList == nullptr)
	{
		return;
	}

	if (Type == Dynamic)
	{
		// allocate directly.
		void* MappedPointer = nullptr;

		Buffer->Map(0, nullptr, &MappedPointer);
		Buffer->Unmap(0, nullptr);
	}
	else
	{
		// allocated by upload pool.
		if (GResourceUpdatePool[UPT_Buffer].PoolIsFull(Size))
		{
			GResourceUpdatePool[UPT_Buffer].FlushAndWaitRequest(RHI, CommandList);
		}
		GResourceUpdatePool[UPT_Buffer].RequestUpload(CommandList, Buffer, InData, Size);
	}
}

uint32 CDX12_VertexBuffer::GetStrideSize(EVertexType InVertexType)
{
	switch (InVertexType)
	{
	case PositionOnly:
		return sizeof(FPositionOnlyVertex);
	}

	return 0;
}
