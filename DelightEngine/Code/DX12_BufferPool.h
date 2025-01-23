#pragma once
#include "Include.h"

#include "comptr.h"
#include "EASTL/list.h"

/*
* upload buffer�� ���⼭.. ũ�� �Ҵ��س��� �߶󾲱�..?
*/
struct FChunkInfo
{
	uint32 PoolIndex;
	uint64 Offset;
	uint64 Size;
};

enum EPoolType
{
	Vertex,
	Index,
};

// @todo : ���߿� scalability�ϰ� �����ϵ��� ����
// 32mb?
const static uint64 BufferpoolSize = 1024 * 1024 * 32;

class CRHIDirectX12;
class CDX12_BufferInterface;
class CDX12_CommandList;

class CDX12_BufferPool
{
public:
	void Initialize(CRHIDirectX12* RHI);
	void Release();

	bool8 AllocateToBuffer(CDX12_CommandList* CommandList, Delight::Comptr<ID3D12Resource> Dest, void* Data, uint64 Size);
	void ClearPool();

	uint64 GetRemainSize();
	bool CanAllocate(uint64 Size);

protected:
	Delight::Comptr<ID3D12Resource> Bufferpool;
	eastl::list<FChunkInfo> AllocateRequestList;

	bool8 bInitializeState = false; // copy_source state.
	byte* PoolData = nullptr;
	uint64 CurrentOffset = 0;
};