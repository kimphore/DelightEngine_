#pragma once

#include <d3d12.h>
#include <DirectXMath.h>

#include "RHI.h"
#include "comptr.h"
#include "Define.h"
#include "Typedef.h"
#include "DX12_CommandList.h"
#include "DX12_Rendertarget.h"
#include "EASTL/list.h"

struct IDXGIFactory2;
struct IDXGIAdapter1;
struct IDXGISwapChain3;
class CDX12_BufferInterface;
class ENGINE_DLL CRHIDirectX12 : CRHIInterface
{
public:
	virtual void Initialize(HWND hWnd);

	virtual void Present(int32 InSyncInterval = 1);
	virtual void WaitForPreviousFrame();
	virtual void Clear(CDX12_CommandList& CommandList) {};

public:
	Delight::Comptr<ID3D12Device> GetDevice();

private:
	void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);

private:
	uint32 m_frameIndex;
	uint32 m_rtvDescriptorSize;
	uint64 m_fenceValue;
	HANDLE m_fenceEvent;

private:
	CDX12_Rendertarget SceneColorBuffer;
	CDX12_Rendertarget SceneDepthBuffer;

public:
	Delight::Comptr<ID3D12Device> m_Device;
	Delight::Comptr<ID3D12CommandQueue> m_CommandQueue;
	Delight::Comptr<IDXGISwapChain3> m_Swapchain;
	Delight::Comptr<ID3D12DescriptorHeap> m_rtvHeap;
	Delight::Comptr<ID3D12Resource> m_RenderTargets[2];

	Delight::Comptr<ID3D12Fence> m_fence;
};