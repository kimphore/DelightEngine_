#include "DX12_ResourceUploadPool.h"
#include "DX12_Functions.h"
#include "DX12_CommandList.h"
#include "RHI_DX12Device.h"
#include "DX12_Resource.h"

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

bool8 CDX12_ResourceUpdatePool::RequestUpload(CDX12_CommandList& CommandList, CDX12_Resource* DestResource, FResourceUploadData& InData)
{
	if (!bInitialized == false)
	{
		D3D12_RESOURCE_BARRIER ResourceBarrier = Delight::DX12::GetTransitionBarrier(PoolResource.GetData(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_SOURCE);

		CommandList->ResourceBarrier(1, &ResourceBarrier);
	}

	if (DestResource && CanAllocate(InData.Data, InData.Size))
	{
		InternalUploadData(CommandList, DestResource, InData);
	}

	return true;
}

void CDX12_ResourceUpdatePool::ClearPool()
{
	AllocateRequestList.clear();
	CurrentOffset = 0;
}

void CDX12_ResourceUpdatePool::FlushAndWaitRequest(CDX12_CommandList& CommandList)
{
	if (CommandQueue.IsValid())
	{
		CommandList.Close();
		CommandList.Execute(CommandQueue);

		ResourceWaitFence.Wait(CommandQueue);

		CommandList.Reset();
		ClearPool();
	}
}

void CDX12_ResourceUpdatePool::InternalUploadData(CDX12_CommandList& CommandList, CDX12_Resource* DestResource, FResourceUploadData& InData)
{
	FChunkInfo NewChunkInfo(0, CurrentOffset, InData.Size);
	void* AllocatePointer = GetAllocatePointer();

	if (DestResource == nullptr)
	{
		return;
	}

	Delight::Memcpy(AllocatePointer, InData.Data, InData.Size);
	
	DestResource->TransitionState(CommandList, D3D12_RESOURCE_STATE_COPY_DEST);
	CommandList->CopyBufferRegion(DestResource->GetResource().GetData(), 0, PoolResource.GetData(), NewChunkInfo.Offset, NewChunkInfo.Size);
	DestResource->TransitionState(CommandList, InData.AfterResourceState);

	AllocateRequestList.push_back(NewChunkInfo);
	CurrentOffset += InData.Size;
}

void* CDX12_ResourceUpdatePool::GetAllocatePointer()
{
	return (void*)(PoolData + CurrentOffset);
}

uint64 CDX12_ResourceUpdatePool::GetRemainSize()
{
	return GetPoolSize() - CurrentOffset;
}

bool8 CDX12_ResourceUpdatePool::CanAllocate(void* Data, uint64 Size)
{
	return Data && !PoolIsFull(Size);
}

D3D12_HEAP_PROPERTIES CDX12_ResourceUpdatePool::GetUploadHeapProperties()
{
	return D3D12_HEAP_PROPERTIES();
}

D3D12_RESOURCE_DESC CDX12_ResourceUpdatePool::GetUploadHeapResourceDesc()
{
	return D3D12_RESOURCE_DESC();
}
