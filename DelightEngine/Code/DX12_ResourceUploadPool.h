#pragma once

#include "Include.h"
#include "comptr.h"
#include "EASTL/list.h"
#include "DX12_Fence.h"

struct FResourceUploadData
{
	void* Data = nullptr;
	uint64 Size = 0;
	uint64 RowPitch = 0;
	uint64 SlicePitch = 0;
	D3D12_RESOURCE_STATES AfterResourceState = D3D12_RESOURCE_STATE_COMMON;
};

enum EUploadPoolType
{
	UPT_Buffer,
	UPT_Texture,
	UPT_Max
};

struct FChunkInfo
{
	uint32 PoolIndex;
	uint64 Offset;
	uint64 Size;

	FChunkInfo(uint32 InPoolIndex, uint64 InOffset, uint64 InSize)
		:PoolIndex(InPoolIndex),
		Offset(InOffset),
		Size(InSize)
	{
	}
};

class CDX12_Resource;
class CDX12_CommandList;
class CDX12_ResourceUpdatePool
{
public:
	void Initialize(Delight::Comptr<ID3D12Device> InDevice, Delight::Comptr<ID3D12CommandQueue> InCommandQueue);
	void Release();

public:
	bool8 RequestUpload(CDX12_CommandList& CommandList, CDX12_Resource* DestResource, FResourceUploadData& InData);
	void ClearPool();
	virtual uint64 GetPoolSize() {
		return 0;
	}
	bool8 PoolIsFull(uint64 Size) {
		return GetRemainSize() <= Size;
	}

	void FlushAndWaitRequest(CDX12_CommandList& CommandList);

protected:
	void InternalUploadData(CDX12_CommandList& CommandList, CDX12_Resource* DestResource, FResourceUploadData& InData);

protected:
	// initialize
	void* GetAllocatePointer();
	uint64 GetRemainSize();
	bool8 CanAllocate(void* Data, uint64 Size);
	virtual D3D12_HEAP_PROPERTIES GetUploadHeapProperties();
	virtual D3D12_RESOURCE_DESC GetUploadHeapResourceDesc();

protected:
	Delight::Comptr<ID3D12Device> Device;
	Delight::Comptr<ID3D12CommandQueue> CommandQueue;
	Delight::Comptr<ID3D12Resource> PoolResource;

	eastl::list<FChunkInfo> AllocateRequestList;

	bool8 bInitialized = false;
	byte* PoolData = nullptr;
	uint64 CurrentOffset = 0;

	CDX12_Fence ResourceWaitFence;

	Delight::Comptr<ID3D12Fence> Fence;
	uint64 FenceValue = 0;
	HANDLE FenceEventHandle;
};

extern CDX12_ResourceUpdatePool GResourceUpdatePool[UPT_Max];