#pragma once

#include <d3d12.h>

#include "RHI.h"
#include "comptr.h"
#include "Define.h"

class CDX12_CommandListPool;
class CRHIDirectX12;
class ENGINE_DLL CDX12_CommandList
{
public:
	CDX12_CommandList(){}
	CDX12_CommandList(const CDX12_CommandList& InList);
	~CDX12_CommandList();

	void Initialize(Delight::Comptr<ID3D12Device> InDevice, CDX12_CommandListPool* InListPool = nullptr);
	void Reset();
	void Close();
	void Clear() {
		commandAllocator.Detach();
		commandList.Detach();
	}

	bool8 IsValid() {
		return commandList.IsValid() && commandAllocator.IsValid();
	}

	void SetPipeline(Delight::Comptr<ID3D12PipelineState> InPipeline);
	void Execute(Delight::Comptr<ID3D12CommandQueue> InQueue);

	Delight::Comptr<ID3D12GraphicsCommandList> operator *()
	{
		return commandList;
	}

	Delight::Comptr<ID3D12GraphicsCommandList> operator ->()
	{
		return commandList;
	}

	CDX12_CommandList& operator = (CDX12_CommandList other);

	Delight::Comptr<ID3D12GraphicsCommandList> Get();

	void SetPoolInfo(CDX12_CommandListPool* InCommandListPool) {
		CommandListPool = InCommandListPool;
	}
	void ClearPoolInfo() {
		CommandListPool = nullptr;
	}

public:
	Delight::Comptr<ID3D12Device> GetDevice() {
		return Device;
	};

private:
	Delight::Comptr<ID3D12Device> Device;
	Delight::Comptr<ID3D12CommandAllocator> commandAllocator;
	Delight::Comptr<ID3D12GraphicsCommandList> commandList;

private:
	CDX12_CommandListPool* CommandListPool = nullptr;
	int32 RefCount = 0;
	bool bInitialized = false;
	bool bClosed = false;	
};