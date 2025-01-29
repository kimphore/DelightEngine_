#include "DX12_ResourceUploadPool.h"
#include "DX12_Functions.h"
#include "DX12_CommandList.h"
#include "RHI_DX12Device.h"

CDX12_ResourceUpdatePool GResourceUpdatePool[UPT_Max];

void CDX12_ResourceUpdatePool::Initialize(CRHIDirectX12* RHI)
{
	if (RHI)
	{
		ID3D12Device* Device = RHI->GetDevice();
		if (Device)
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

			FenceValue = 0;
			Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, DELIGHT_IID_PPV_ARGS(&Fence));
			FenceEventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		}
	}

	if (PoolResource.IsValid())
	{
		PoolResource->Map(0, nullptr, (void**)&PoolData);
	}
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

void CDX12_ResourceUpdatePool::FlushAndWaitRequest(CRHIDirectX12* RHI, CDX12_CommandList* CommandList)
{
	if (RHI && CommandList)
	{
		CommandList->Execute(RHI->m_CommandQueue);
		++FenceValue;
		RHI->m_CommandQueue->Signal(Fence.GetData(), FenceValue);

		if (Fence->GetCompletedValue() < FenceValue)
		{
			Fence->SetEventOnCompletion(FenceValue, FenceEventHandle);
			WaitForSingleObject(FenceEventHandle, INFINITE);

			CommandList->Reset();
			ClearPool();
		}
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
