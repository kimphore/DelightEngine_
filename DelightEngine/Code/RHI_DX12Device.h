#pragma once

#include <d3d12.h>
#include <DirectXMath.h>

#include "RHI.h"
#include "comptr.h"
#include "Define.h"
#include "Typedef.h"

#include "EASTL/list.h"

#include "DX12_CommandList.h"
#include "DX12_Rendertarget.h"
#include "DX12_Fence.h"

static const uint32 GNumBackbuffer = 2;

enum ECommandQueueType
{
	CQT_Direct,
	CQT_Compute,
	CQT_Copy,// 안쓸듯..
	CQT_Max,
};

struct IDXGIFactory2;
struct IDXGIAdapter1;
struct IDXGISwapChain3;
class CDX12_BufferInterface;
class CDX12_Resource;
class ENGINE_DLL CRHIDirectX12 : CRHIInterface
{
public:
	virtual void Initialize(HWND hWnd);

	virtual void Present(int32 InSyncInterval = 1);
	virtual void WaitForPreviousFrame();
	virtual void Clear(CDX12_CommandList& CommandList) {};
	void EndFrame();

	CDX12_Rendertarget& GetBackbuffer();

public:
	void BindDescriptionHeaps(CDX12_CommandList& CommandList);

public:
	void AddPendingDeleteUploadResource(CDX12_Resource* InResource);

private:
	// initialize manager, pool i've created.
	void ComponentInitialize();
	void CommandQueueInitialize();
	void RenderTargetInitialize();
	void DeleteUploadResources();

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

	// 삭제해야 할 업로드버퍼들.(현재는 텍스쳐만..)
private:
	eastl::list<CDX12_Resource*> PendingDeleteUploadResources;
};

extern ENGINE_DLL CRHIDirectX12* GRHI;