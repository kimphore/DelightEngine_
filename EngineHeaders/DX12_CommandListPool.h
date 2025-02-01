#pragma once

#include "Include.h"
#include "DX12_CommandList.h"
#include "EASTL/queue.h"

class CRHIDirectX12;
class CDX12_CommandListPool
{
public:
	void Initialize(Delight::Comptr<ID3D12Device> InDevice);

public:
	void GetCommandList(CDX12_CommandList& InCommandList);
	void ReturnToPool(const CDX12_CommandList* InList);
private:
	Delight::Comptr<ID3D12Device> Device;
	eastl::queue<CDX12_CommandList> Pool;
};

extern CDX12_CommandListPool GCommandListPool;