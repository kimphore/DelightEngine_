#include "DX12_CommandListPool.h"
#include "RHI_DX12Device.h"

static const uint32 ReservedNumCommnadList = 10;
CDX12_CommandListPool GCommandListPool;

/*
* Return To Pool해도 포인터는 남아있으므로 초기화 X
* Get으로 가져올 때, 무조건 리셋 후 사용하도록.
*/

void CDX12_CommandListPool::Initialize(Delight::Comptr<ID3D12Device> InDevice)
{
	Device = InDevice;

	for (int32 i = 0; i < ReservedNumCommnadList; ++i)
	{
		CDX12_CommandList NewCommandList;

		NewCommandList.Initialize(Device, nullptr);
		Pool.emplace_back(NewCommandList);
	}
}

CDX12_CommandList CDX12_CommandListPool::GetCommandList()
{
	CDX12_CommandList Temp;

	if (Pool.empty())
	{
		Temp.Initialize(Device, nullptr);
	}
	else
	{
		Temp = Pool.front();
		Pool.pop();
	}
	Temp.SetPoolInfo(this); // pool 밖에서만 리턴을 위해 정보 할당

	return Temp;
}

void CDX12_CommandListPool::ReturnToPool(const CDX12_CommandList* InList)
{
	CDX12_CommandList Temp(*InList);

	Temp.ClearPoolInfo();
	UsedPool.emplace_back(Temp);
}

void CDX12_CommandListPool::ResetPool()
{
	while (!UsedPool.empty())
	{
		Pool.emplace_back(UsedPool.front());
		UsedPool.pop();
	}
}

