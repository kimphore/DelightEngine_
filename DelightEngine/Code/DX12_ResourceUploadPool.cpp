#include "DX12_ResourceUploadPool.h"
#include "DX12_Functions.h"
#include "DX12_CommandList.h"
#include "RHI_DX12Device.h"

CDX12_ResourceUpdatePool GResourceUpdatePool[UPT_Max];

void CDX12_ResourceUpdatePool::Initialize(Delight::Comptr<ID3D12Device> InDevice, Delight::Comptr<ID3D12CommandQueue> InCommandQueue)
{
	Device = InDevice;
	CommandQueue = InCommandQueue;
	if (Device.IsValid())
	{
		D3D12_HEAP_PROPERTIES HeapProperty = GetUploadHeapProperties();
		D3D12_RESOURCE_DESC ResourceDesc = GetUploadHeapResourceDesc();

		Device->CreateCommittedResource(
			&HeapProperty,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			DELIGHT_IID_PPV_ARGS(&PoolResource));
	}

	if (PoolResource.IsValid())
	{
		PoolResource->Map(0, nullptr, (void**)&PoolData);
	}

	ResourceWaitFence.Initialize(Device);
}

void CDX12_ResourceUpdatePool::Release()
{
	if (PoolData && PoolResource.IsValid())
	{
		PoolResource->Unmap(0, nullptr);
		PoolData = nullptr;
		bInitialized = false;
	}
}

bool8 CDX12_ResourceUpdatePool::RequestUpload(CDX12_CommandList* CommandList, Delight::Comptr<ID3D12Resource> Dest, void* Data, uint64 Size)
{
	if (CommandList == nullptr)
	{
		return false;
	}

	if (!bInitialized == false)
	{
		D3D12_RESOURCE_BARRIER ResourceBarrier = Delight::DX12::GetTransitionBarrier(PoolResource.GetData(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_SOURCE);

		CommandList->Get()->ResourceBarrier(1, &ResourceBarrier);
	}

	if (CanAllocate(Dest, Data, Size))
	{
		FChunkInfo NewChunkInfo(0, CurrentOffset, Size);
		void* AllocatePointer = GetAllocatePointer();

		Delight::Memcpy(AllocatePointer, Data, Size);

		D3D12_RESOURCE_BARRIER ResourceBarrier = Delight::DX12::GetTransitionBarrier(Dest.GetData(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);

		CommandList->Get()->ResourceBarrier(1, &ResourceBarrier);
		CommandList->Get()->CopyBufferRegion(Dest.GetData(), 0, PoolResource.GetData(), NewChunkInfo.Offset, NewChunkInfo.Size);

		ResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		ResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

		CommandList->Get()->ResourceBarrier(1, &ResourceBarrier);

		AllocateRequestList.push_back(NewChunkInfo);
		CurrentOffset += Size;		
	}

	return true;
}

void CDX12_ResourceUpdatePool::ClearPool()
{
	AllocateRequestList.clear();
	CurrentOffset = 0;
}

void CDX12_ResourceUpdatePool::FlushAndWaitRequest(CDX12_CommandList* CommandList)
{
	if (CommandList && CommandQueue.IsValid())
	{
		CommandList->Close();
		CommandList->Execute(CommandQueue);

		ResourceWaitFence.Wait(CommandQueue);

		CommandList->Reset();
		ClearPool();
	}
}

void* CDX12_ResourceUpdatePool::GetAllocatePointer()
{
	return (void*)(PoolData + CurrentOffset);
}

uint64 CDX12_ResourceUpdatePool::GetRemainSize()
{
	return GetPoolSize() - CurrentOffset;
}

bool8 CDX12_ResourceUpdatePool::CanAllocate(Delight::Comptr<ID3D12Resource> Target, void* Data, uint64 Size)
{
	return Target.IsValid() && Data && !PoolIsFull(Size);
}

D3D12_HEAP_PROPERTIES CDX12_ResourceUpdatePool::GetUploadHeapProperties()
{
	return D3D12_HEAP_PROPERTIES();
}

D3D12_RESOURCE_DESC CDX12_ResourceUpdatePool::GetUploadHeapResourceDesc()
{
	return D3D12_RESOURCE_DESC();
}
