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
#include "DX12_Fence.h"

static const uint32 GNumBackbuffer = 2;

enum ECommandQueueType
{
	CQT_Direct,
	CQT_Compute,
	CQT_Copy,// ¾È¾µµí..
	CQT_Max,
};

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

	CDX12_Rendertarget& GetBackbuffer();

public:
	void BindDescriptionHeaps(CDX12_CommandList& CommandList);

private:
	// initialize manager, pool i've created.
	void ComponentInitialize();
	void CommandQueueInitialize();
	void RenderTargetInitialize();

public:
	Delight::Comptr<ID3D12Device> GetDevice();
	Delight::Comptr<ID3D12CommandQueue> GetCommandQueue(ECommandQueueType InType = CQT_Direct);

private:
	void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);

private:
	uint32 FrameIndex;

private:
	CDX12_Rendertarget Backbuffers[GNumBackbuffer];
	CDX12_Rendertarget SceneColorBuffer;
	CDX12_Rendertarget SceneDepthBuffer;

private:
	CDX12_Fence FrameFence;

private:
	Delight::Comptr<ID3D12Device> Device;
	Delight::Comptr<ID3D12CommandQueue> CommandQueue[CQT_Max];
	Delight::Comptr<IDXGISwapChain3> SwapChain;
};