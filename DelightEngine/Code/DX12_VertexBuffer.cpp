#include "DX12_VertexBuffer.h"

#include "Include.h"
#include "RHI_DX12Device.h"
#include "DX12_ResourceUploadPool.h"

bool8 CDX12_VertexBuffer::CreateBuffer(Delight::Comptr<ID3D12Device> InDevice, EBufferType InType, EVertexType InVertexType, uint32 InSize)
{
	if (InSize > 0)
	{
		Device = InDevice;
		if (Device.IsValid())
		{
			Type = InType;
			Size = InSize;
			VertexType = InVertexType;

			D3D12_HEAP_PROPERTIES HeapProperty; // �۷ι��� ������

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
				DELIGHT_IID_PPV_ARGS(&Resource));

			VertexBufferView.BufferLocation = Resource->GetGPUVirtualAddress();
			VertexBufferView.StrideInBytes = GetStrideSize(VertexType);
			VertexBufferView.SizeInBytes = Size;

			return true;
		}
	}

	return false;
}

void CDX12_VertexBuffer::SetData(CDX12_CommandList& CommandList, void* InData, uint64 Size)
{
	if (!Resource.IsValid() || InData == nullptr || Size <= 0)
	{
		return;
	}

	if (Type == Dynamic)
	{
		// allocate directly.
		void* MappedPointer = nullptr;

		Resource->Map(0, nullptr, &MappedPointer);
		Resource->Unmap(0, nullptr);
	}
	else
	{
		// allocated by upload pool.
		if (GResourceUpdatePool[UPT_Buffer].PoolIsFull(Size))
		{
			GResourceUpdatePool[UPT_Buffer].FlushAndWaitRequest(CommandList);
		}

		FResourceUploadData UploadData = {};
		UploadData.Data = InData;
		UploadData.AfterResourceState = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		UploadData.Size = Size;

		GResourceUpdatePool[UPT_Buffer].RequestUpload(CommandList, this, UploadData);
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
