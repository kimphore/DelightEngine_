#include "DX12_DescriptorHeapManager.h"
#include "RHI_DX12Device.h"

CDX12_DescriptorHeapManager GDescriptorHeapManager;

void CDX12_DescriptorHeapManager::Initialize(Delight::Comptr<ID3D12Device> InDevice)
{
	Device = InDevice;
	if (Device.IsValid())
	{
		for (int32 i = 0; i < DHT_Max; ++i)
		{
			D3D12_DESCRIPTOR_HEAP_DESC HeapDesc = GetHeapDesc(i);
			FDescriptorHeap& Heap = Heaps[i];

			Heap.bUseShader = i >= DHT_ShaderResource;
			Heap.MaxNumDescriptor = HeapDesc.NumDescriptors;
			Heap.NumDescriptor = 0;
			Delight::ThrowIfFailed(Device->CreateDescriptorHeap(&HeapDesc, DELIGHT_IID_PPV_ARGS(&Heap.Heap)));

			Heap.Stride = Device->GetDescriptorHandleIncrementSize(HeapDesc.Type);
			Heap.CurrentPtr.CPUHandle = Heap.Heap->GetCPUDescriptorHandleForHeapStart();
			if (i > DHT_DSV) // only shader resource.
			{
				Heap.CurrentPtr.GPUHandle = Heap.Heap->GetGPUDescriptorHandleForHeapStart();
			}
		}
	}
}

FDescriptorHandleSet CDX12_DescriptorHeapManager::GetHandle(int32 Index)
{
	FDescriptorHandleSet Handle = {};

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

void CDX12_DescriptorHeapManager::ReturnToQueue(int32 Index, FDescriptorHandleSet& HandleSet)
{
	if (IsValidIndex(Index))
	{
		FDescriptorHeap& Heap = Heaps[Index];

		Heap.ResuableHandleQueue.push(HandleSet);
	}
}

D3D12_DESCRIPTOR_HEAP_DESC CDX12_DescriptorHeapManager::GetHeapDesc(int32 Index)
{
	D3D12_DESCRIPTOR_HEAP_DESC Result = {};

	switch (Index)
	{
	case DHT_RTV:
		Result.NumDescriptors = 32;
		Result.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		Result.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		break;
	case DHT_DSV:
		Result.NumDescriptors = 4;
		Result.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		Result.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		break;
	case DHT_ShaderResource:
		Result.NumDescriptors = 4096;
		Result.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		Result.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		break;
	case DHT_Sampler:
		Result.NumDescriptors = 1024;
		Result.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		Result.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		break;
	}

	return Result;
}

bool8 CDX12_DescriptorHeapManager::IsValidIndex(int32 Index)
{
	return Index > INDEX_NONE && Index < DHT_Max;
}

