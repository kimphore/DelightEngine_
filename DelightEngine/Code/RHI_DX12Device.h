#pragma once

#include "RHI.h"
#include "comptr.h"

class ENGINE_DLL CRHIDirectX12 : CRHIInterface
{
public:
	virtual void Initialize(HWND hWnd);
private:
	void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);

private:
	uint32 m_frameIndex;
	uint32 m_rtvDescriptorSize;
public:
	Delight::Comptr<ID3D12Device> m_Device;
	Delight::Comptr<ID3D12CommandQueue> m_CommandQueue;
	Delight::Comptr<IDXGISwapChain3> m_Swapchain;
	Delight::Comptr<ID3D12DescriptorHeap> m_rtvHeap;
	Delight::Comptr<ID3D12Resource> m_RenderTargets[2];
	Delight::Comptr<ID3D12CommandAllocator> m_commandAllocator;
	Delight::Comptr<ID3D12GraphicsCommandList> m_commandList;
};