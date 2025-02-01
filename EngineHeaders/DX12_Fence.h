#pragma once

#include "Include.h"

class CRHIDirectX12;
class CDX12_Fence
{
public:
	void Initialize(Delight::Comptr<ID3D12Device> InDevice);
	void Wait(Delight::Comptr<ID3D12CommandQueue> InCommandQueue);
	
private:
	Delight::Comptr<ID3D12Device> Device;
	Delight::Comptr<ID3D12Fence> Fence;

	HANDLE FenceEvent;
	uint64 FenceValue;
};