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
	CDX12_CommandList GetCommandList();
	void GetCommandList(CDX12_CommandList& InCommandList);
	void ReturnToPool(const CDX12_CommandList* InList);
	void ResetPool();
private:
	Delight::Comptr<ID3D12Device> Device;
	eastl::queue<CDX12_CommandList> Pool;
	eastl::queue<CDX12_CommandList> UsedPool; // 현재 프레임에서 사용한 애들은 다음프레임까지 사용 X
};

extern CDX12_CommandListPool GCommandListPool;