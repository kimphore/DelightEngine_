#include "DX12_Buffer.h"
#include "DX12_ResourceUploadPool.h"

bool8 CDX12_BufferInterface::CreateBuffer(Delight::Comptr<ID3D12Device> InDevice, EBufferType InType, uint32 InStride, uint32 InSize)
{
	if (InDevice.IsValid())
	{
		Device = InDevice;
		Type = InType;
		Size = InSize;
		Stride = InStride;

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
		ResourceDesc.MipLevels = 1; // auto calc
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

		CreateBufferView();
		
		return true;
	}
	
	return false;
}

void CDX12_BufferInterface::SetData(CDX12_CommandList& CommandList, void* InData, uint64 InSize)
{
	if (!Resource.IsValid() || InData == nullptr || InSize <= 0)
	{
		return;
	}

	if (Type == Dynamic)
	{
		// allocate directly.
		void* MappedPointer = nullptr;

		Resource->Map(0, nullptr, &MappedPointer);
		Delight::Memcpy(MappedPointer, InData, InSize);
		Resource->Unmap(0, nullptr);
	}
	else
	{
		// allocated by upload pool.
		EUploadPoolType PoolType = GetUploadPoolType();
		if (GResourceUpdatePool[PoolType].PoolIsFull(InSize))
		{
			GResourceUpdatePool[PoolType].FlushAndWaitRequest(CommandList);
		}

		FResourceUploadData UploadData = {};
		UploadData.Data = InData;
		UploadData.AfterResourceState = (PoolType == UPT_IndexBuffer) ? D3D12_RESOURCE_STATE_INDEX_BUFFER : D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		UploadData.Size = InSize;

		GResourceUpdatePool[PoolType].RequestUpload(CommandList, this, UploadData);
	}
}

