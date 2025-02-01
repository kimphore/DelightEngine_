#include "DX12_Fence.h"
#include "RHI_DX12Device.h"

void CDX12_Fence::Initialize(Delight::Comptr<ID3D12Device> InDevice)
{
	Device = InDevice;
	if (Device.IsValid())
	{
		FenceValue = 0;
		Device->CreateFence(FenceValue, D3D12_FENCE_FLAG_NONE, DELIGHT_IID_PPV_ARGS(&Fence));
		FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	}
}

void CDX12_Fence::Wait(Delight::Comptr<ID3D12CommandQueue> InCommandQueue)
{
	if (InCommandQueue.IsValid())
	{
		++FenceValue;
		InCommandQueue->Signal(Fence.GetData(), FenceValue);
		if (Fence->GetCompletedValue() < FenceValue)
		{
			Fence->SetEventOnCompletion(FenceValue, FenceEvent);
			WaitForSingleObject(FenceEvent, INFINITE);
		}
	}
}

