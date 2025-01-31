#pragma once

#include "Include.h"
#include "EASTL/list.h"
#include "EASTL/queue.h"

enum EDescriptorHeapType
{
	RTV,
	DSV,
	ShaderResource, // CBV SRV UAV
	Sampler,
	DHT_Max
};

struct FDescriptorHandleSet
{
	D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle;

	FDescriptorHandleSet& operator += (uint32 Value)
	{
		CPUHandle.ptr += Value;
		GPUHandle.ptr += Value;

		return *this;
	}
};

struct FDescriptorHeap
{
	Delight::Comptr<ID3D12DescriptorHeap> Heap;
	FDescriptorHandleSet CurrentPtr;
	eastl::queue<FDescriptorHandleSet> ResuableHandleQueue;

	uint64 Stride;
	uint32 MaxNumDescriptor;
	uint32 NumDescriptor;

	void NextOffset()
	{
		CurrentPtr += Stride;
	}
};

class CRHIDirectX12;
class CDX12_DescriptorHeapManager
{
public:
	void Initialize(CRHIDirectX12* RHI);

	FDescriptorHandleSet GetHandle(uint32 Index);
	void ReturnToQueue(uint32 Index, FDescriptorHandleSet& HandleSet);

protected:
	D3D12_DESCRIPTOR_HEAP_DESC GetHeapDesc(uint32 Index);
	bool8 IsValidIndex(uint32 Index);

protected:
	Delight::Comptr<ID3D12Device> Device;

protected:
	FDescriptorHeap Heaps[DHT_Max];
};

extern CDX12_DescriptorHeapManager GDescriptorHeapManager;