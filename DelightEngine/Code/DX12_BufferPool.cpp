#include "DX12_BufferPool.h"
#include "RHI_DX12Device.h"
#include "DX12_CommandList.h"

void CDX12_BufferPool::Initialize(CRHIDirectX12* RHI)
{
	if (RHI)
	{
		ID3D12Device* Device = RHI->GetDevice();
		if (Device)
		{
			D3D12_HEAP_PROPERTIES HeapProperty; // 글로벌로 뺄수도

			HeapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
			HeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			HeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			HeapProperty.CreationNodeMask = 1;
			HeapProperty.VisibleNodeMask = 1;

			D3D12_RESOURCE_DESC ResourceDesc;

			ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			ResourceDesc.Alignment = 0; // default 64kb
			ResourceDesc.Width = BufferpoolSize;
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
				DELIGHT_IID_PPV_ARGS(&Bufferpool));
		}
	}

	if (Bufferpool.IsValid())
	{
		Bufferpool->Map(0, nullptr, (void**)&PoolData);
	}
}

void CDX12_BufferPool::Release()
{
	if (PoolData && Bufferpool.IsValid())
	{
		Bufferpool->Unmap(0, nullptr);
		PoolData = nullptr;
	}
}

bool8 CDX12_BufferPool::AllocateToBuffer(CDX12_CommandList* CommandList, Delight::Comptr<ID3D12Resource> Dest, void* Data, uint64 Size)
{
	if (CommandList == nullptr)
	{
		return;
	}

	if (!bInitializeState)
	{
		D3D12_RESOURCE_BARRIER ResourceBarrier = {};
		ResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		ResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		ResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		ResourceBarrier.Transition.pResource = Dest.GetData();
		ResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
		ResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
		
		CommandList->Get()->ResourceBarrier(1, &ResourceBarrier);

		bInitializeState = true;
	}

	if (Data && Dest.IsValid() && CanAllocate(Size))
	{
		FChunkInfo Info;
		Info.PoolIndex = 0; // not use yet
		Info.Offset = CurrentOffset;
		Info.Size = Size;

		void* AllocPointer = (void*)(PoolData + CurrentOffset);

		Delight::Memcpy(AllocPointer, Data, Size);

		D3D12_RESOURCE_BARRIER ResourceBarrier = {};
		ResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		ResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		ResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		ResourceBarrier.Transition.pResource = Dest.GetData();
		ResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
		ResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;

		CommandList->Get()->ResourceBarrier(1, &ResourceBarrier);
		CommandList->Get()->CopyBufferRegion(Dest.GetData(), 0, Bufferpool.GetData(), Info.Offset, Info.Size);

		ResourceBarrier.Transition.pResource = Dest.GetData();
		ResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		ResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

		CommandList->Get()->ResourceBarrier(1, &ResourceBarrier);

		AllocateRequestList.push_back(Info); // 느낌상 필요없을거같은데
		CurrentOffset += Size;
	}
}

void CDX12_BufferPool::ClearPool()
{
	CurrentOffset = 0;
	AllocateRequestList.clear();
}

uint64 CDX12_BufferPool::GetRemainSize()
{
	return BufferpoolSize - CurrentOffset;
}

bool CDX12_BufferPool::CanAllocate(uint64 Size)
{
	return PoolData && Size > 0 && Size < GetRemainSize();
}

