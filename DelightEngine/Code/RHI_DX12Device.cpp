#include "include.h"
#include "comptr.h"
#include "RHI_DX12Device.h"
#include "DirectXLibs/d3dx12.h"
#include "dxgi1_4.h"

#include "DX12_DescriptorHeapManager.h"
#include "DX12_CommandListPool.h"

void CRHIDirectX12::Initialize(HWND hWnd)
{
	UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
	Delight::Comptr<ID3D12Debug> debugController;

	if (SUCCEEDED(D3D12GetDebugInterface(DELIGHT_IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	}
#endif

	Delight::Comptr<IDXGIFactory4> factory;
	Delight::FailedReturnString(CreateDXGIFactory2(dxgiFactoryFlags, DELIGHT_IID_PPV_ARGS(&factory)),
		TEXT("Failed Create DXGI Factory."));

	Delight::Comptr<IDXGIAdapter1> hardwareAdapter;
	
	GetHardwareAdapter(factory.GetData(), &hardwareAdapter);

	Delight::ThrowIfFailed(D3D12CreateDevice(hardwareAdapter.GetData(), 
		D3D_FEATURE_LEVEL_11_0, DELIGHT_IID_PPV_ARGS(&Device)));

	CommandQueueInitialize();

	// Creat SwapChain.
	DXGI_SWAP_CHAIN_DESC1 chainDesc = {};
	chainDesc.BufferCount = GNumBackbuffer;
	chainDesc.Width = 1280;
	chainDesc.Height = 720;
	chainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.SampleDesc.Count = 1; // multisampling off..?

	Delight::Comptr<IDXGISwapChain1> TempSwapChain;

	Delight::FailedReturnString(factory->CreateSwapChainForHwnd(
		GetCommandQueue().GetData(),
		hWnd,
		&chainDesc,
		nullptr,
		nullptr,
		&TempSwapChain
		), TEXT("Failed Create Swap Chain."));

	// Not Provide Full Screen... Tmp..
	Delight::ThrowIfFailed(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));
	Delight::ThrowIfFailed(TempSwapChain->QueryInterface(__uuidof(IDXGISwapChain3), reinterpret_cast<void**>(&SwapChain)));

	FrameIndex = SwapChain->GetCurrentBackBufferIndex();

	ComponentInitialize();
	RenderTargetInitialize();

	FrameFence.Initialize(GetDevice());
}

void CRHIDirectX12::Present(int32 InSyncInterval)
{
	Delight::ThrowIfFailed(SwapChain->Present(InSyncInterval, 0));
}

void CRHIDirectX12::WaitForPreviousFrame()
{
	FrameFence.Wait(GetCommandQueue());
	FrameIndex = SwapChain->GetCurrentBackBufferIndex();
}

CDX12_Rendertarget& CRHIDirectX12::GetBackbuffer()
{
	return Backbuffers[FrameIndex % GNumBackbuffer];
}

void CRHIDirectX12::ComponentInitialize()
{
	extern CDX12_DescriptorHeapManager GDescriptorHeapManager;
	GDescriptorHeapManager.Initialize(GetDevice());

	extern CDX12_CommandListPool GCommandListPool;
	GCommandListPool.Initialize(GetDevice());
}

void CRHIDirectX12::CommandQueueInitialize()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	Delight::ThrowIfFailed(Device->CreateCommandQueue(&queueDesc, DELIGHT_IID_PPV_ARGS(&CommandQueue[CQT_Direct])));

	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;

	Delight::ThrowIfFailed(Device->CreateCommandQueue(&queueDesc, DELIGHT_IID_PPV_ARGS(&CommandQueue[CQT_Compute])));

	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;

	Delight::ThrowIfFailed(Device->CreateCommandQueue(&queueDesc, DELIGHT_IID_PPV_ARGS(&CommandQueue[CQT_Copy])));
}

void CRHIDirectX12::RenderTargetInitialize()
{
	for (uint32 i = 0; i < GNumBackbuffer; ++i)
	{
		Delight::Comptr<ID3D12Resource> Buffer;

		SwapChain->GetBuffer(i, DELIGHT_IID_PPV_ARGS(&Buffer));
		Backbuffers[i].Initialize(GetDevice(), Buffer);
	}
}

Delight::Comptr<ID3D12Device> CRHIDirectX12::GetDevice()
{
	return Device;
}

Delight::Comptr<ID3D12CommandQueue> CRHIDirectX12::GetCommandQueue(ECommandQueueType InType /*= CQT_Direct*/)
{
	if (InType > INDEX_NONE && InType < CQT_Max)
	{
		return CommandQueue[InType];
	}

	return CommandQueue[0];
}

void CRHIDirectX12::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
	Delight::Comptr<IDXGIAdapter1> adapter;	
	Delight::Comptr<IDXGIFactory6> Factory6;
	*ppAdapter = nullptr;

	if (SUCCEEDED(pFactory->QueryInterface(DELIGHT_IID_PPV_ARGS(&Factory6))))
	{
		for (
			UINT adapterIndex = 0;
			SUCCEEDED(Factory6->EnumAdapterByGpuPreference(
				adapterIndex,
				DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
				IID_PPV_ARGS(&adapter)));
				++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				// If you want a software adapter, pass in "/warp" on the command line.
				continue;
			}

			// Check to see whether the adapter supports Direct3D 12, but don't create the
			// actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(adapter.GetData(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}
	}

	if (adapter.GetData() == nullptr)
	{
		for (uint32 index = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(index, &adapter); ++index)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// can't use hardware adapter..
				continue;
			}

			if (SUCCEEDED(D3D12CreateDevice(adapter.GetData(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}
	}
	
	*ppAdapter = adapter.Detach();
}

