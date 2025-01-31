#include "DX12_DescriptorHeapManager.h"
#include "RHI_DX12Device.h"

CDX12_DescriptorHeapManager GDescriptorHeapManager;

void CDX12_DescriptorHeapManager::Initialize(CRHIDirectX12* RHI)
{
	if (RHI)
	{
		Device = RHI->GetDevice();
		if (Device.IsValid())
		{
			for (uint32 i = 0; i < DHT_Max; ++i)
			{
				D3D12_DESCRIPTOR_HEAP_DESC HeapDesc = GetHeapDesc(i);
				FDescriptorHeap& Heap = Heaps[i];

				Heap.MaxNumDescriptor = HeapDesc.NumDescriptors;
				Heap.NumDescriptor = 0;
				Delight::ThrowIfFailed(Device->CreateDescriptorHeap(&HeapDesc, DELIGHT_IID_PPV_ARGS(&Heap.Heap)));

				Heap.Stride = Device->GetDescriptorHandleIncrementSize(HeapDesc.Type);
				Heap.CurrentPtr.CPUHandle = Heap.Heap->GetCPUDescriptorHandleForHeapStart();
				if (i > DSV) // only shader resource.
				{
					Heap.CurrentPtr.GPUHandle = Heap.Heap->GetGPUDescriptorHandleForHeapStart();
				}
			}
		}
	}
}

FDescriptorHandleSet CDX12_DescriptorHeapManager::GetHandle(uint32 Index)
{
	FDescriptorHandleSet Handle;

	if (IsValidIndex(Index))
	{
		FDescriptorHeap& Heap = Heaps[Index];

		if (!Heap.ResuableHandleQueue.empty())
		{
			Handle = Heap.ResuableHandleQueue.front();
			Heap.ResuableHandleQueue.pop();
		}
		else
		{
			Handle = Heap.CurrentPtr;
			Heap.NextOffset();
		} 
	}

	return Handle;
}

void CDX12_DescriptorHeapManager::ReturnToQueue(uint32 Index, FDescriptorHandleSet& HandleSet)
{
	if (IsValidIndex(Index))
	{
		FDescriptorHeap& Heap = Heaps[Index];

		Heap.ResuableHandleQueue.push(HandleSet);
	}
}

D3D12_DESCRIPTOR_HEAP_DESC CDX12_DescriptorHeapManager::GetHeapDesc(uint32 Index)
{
	D3D12_DESCRIPTOR_HEAP_DESC Result = {};

	switch (Index)
	{
	case RTV:
		Result.NumDescriptors = 32;
		Result.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		Result.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		break;
	case DSV:
		Result.NumDescriptors = 4;
		Result.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		Result.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		break;
	case ShaderResource:
		Result.NumDescriptors = 2048;
		Result.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		Result.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		break;
	case Sampler:
		break;
	}
}

bool8 CDX12_DescriptorHeapManager::IsValidIndex(uint32 Index)
{
	return Index > INDEX_NONE && Index < DHT_Max;
}

