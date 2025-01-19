#pragma once

#include <d3d12.h>

#include "RHI.h"
#include "comptr.h"
#include "Define.h"

class CRHIDirectX12;
class ENGINE_DLL CDX12_CommandList
{
public:
	void Init(CRHIDirectX12* InRHI);
	void Reset();
	void Close();

	void SetPipeline(Delight::Comptr<ID3D12PipelineState> InPipeline);
	void Execute(Delight::Comptr<ID3D12CommandQueue> InQueue);

	Delight::Comptr<ID3D12GraphicsCommandList> Get();
private:
	Delight::Comptr<ID3D12CommandAllocator> commandAllocator;
	Delight::Comptr<ID3D12GraphicsCommandList> commandList;
	bool bInitialized = false;
};