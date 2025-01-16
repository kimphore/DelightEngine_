#pragma once

#include <d3d12.h>

#include "RHI.h"
#include "comptr.h"
#include "Define.h"

class ENGINE_DLL CDX12_CommandList
{
public:
	void Init(Delight::Comptr<ID3D12Device> InDevice);
	void Reset();

	Delight::Comptr<ID3D12GraphicsCommandList> Get();
private:
	Delight::Comptr<ID3D12CommandAllocator> m_commandAllocator;
	Delight::Comptr<ID3D12GraphicsCommandList> m_commandList;
	bool bInitialized = false;
};