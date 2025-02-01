#pragma once

#include "Include.h"
#include "EASTL/list.h"
#include "EASTL/queue.h"

enum EDescriptorHeapType
{
	DHT_RTV,
	DHT_DSV,
	DHT_ShaderResource, // CBV SRV UAV
	DHT_Sampler,
	DHT_Max
};

struct FDescriptorHandleSet
{
	D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle;

	void AddOffset(uint32 Value, bool CPUHandleOnly = false)
	{
		CPUHandle.ptr += Value;
		if (CPUHandleOnly)
		{
			GPUHandle.ptr += Value;
		}
	}
};

struct FDescriptorHeap
{
	Delight::Comptr<ID3D12DescriptorHeap> Heap;
	FDescriptorHandleSet CurrentPtr;
	eastl::queue<FDescriptorHandleSet> ResuableHandleQueue;

	uint32 Stride = 0;
	uint32 MaxNumDescriptor = 0;
	uint32 NumDescriptor = 0;
	bool8 bUseShader = false;

	void NextOffset()
	{
		CurrentPtr.AddOffset(Stride, !bUseShader);
	}
};

class CRHIDirectX12;
class CDX12_DescriptorHeapManager
{
public:
	void Initialize(Delight::Comptr<ID3D12Device> InDevice);

	FDescriptorHandleSet GetHandle(int32 Index);
	void ReturnToQueue(int32 Index, FDescriptorHandleSet& HandleSet);

protected:
	D3D12_DESCRIPTOR_HEAP_DESC GetHeapDesc(int32 Index);
	bool8 IsValidIndex(int32 Index);

protected:
	Delight::Comptr<ID3D12Device> Device;

protected:
	FDescriptorHeap Heaps[DHT_Max];
};

extern CDX12_DescriptorHeapManager GDescriptorHeapManager;