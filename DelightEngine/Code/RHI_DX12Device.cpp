#include "include.h"
#include "comptr.h"
#include "RHI_DX12Device.h"
#include "DirectXLibs/d3dx12.h"
#include "dxgi1_4.h"

#include "DX12_DescriptorHeapManager.h"

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
		D3D_FEATURE_LEVEL_11_0, DELIGHT_IID_PPV_ARGS(&m_Device)));

	// Create Command Queue.

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	Delight::FailedReturnString(m_Device->CreateCommandQueue(&queueDesc, DELIGHT_IID_PPV_ARGS(&m_CommandQueue)),
		TEXT("Failed Create Command Queue."));

	// Creat SwapChain.
	DXGI_SWAP_CHAIN_DESC1 chainDesc = {};
	chainDesc.BufferCount = GNumBackbuffer;
	chainDesc.Width = 1280;
	chainDesc.Height = 720;
	chainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.SampleDesc.Count = 1; // multisampling off..?

	Delight::Comptr<IDXGISwapChain1> swapChain;

	Delight::FailedReturnString(factory->CreateSwapChainForHwnd(
		m_CommandQueue.GetData(),
		hWnd,
		&chainDesc,
		nullptr,
		nullptr,
		&swapChain
		), TEXT("Failed Create Swap Chain."));

	// Not Provide Full Screen... Tmp..
	Delight::FailedReturnString(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER), TEXT(""));
	Delight::FailedReturnString(swapChain->QueryInterface(__uuidof(IDXGISwapChain3), reinterpret_cast<void**>(&m_Swapchain)), TEXT(""));

	FrameIndex = m_Swapchain->GetCurrentBackBufferIndex();

	extern CDX12_DescriptorHeapManager GDescriptorHeapManager;
	GDescriptorHeapManager.Initialize(this);
	
	// initialize backbuffer
	for (uint32 i = 0; i < GNumBackbuffer; ++i)
	{
		Delight::Comptr<ID3D12Resource> Buffer;

		m_Swapchain->GetBuffer(i, DELIGHT_IID_PPV_ARGS(&Buffer));
		Backbuffers[i].Initialize(this, Buffer);
	}

	// fence 임시생성
	m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, DELIGHT_IID_PPV_ARGS(&m_fence));
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	MainCommandList.Init(this);
}

void CRHIDirectX12::Present(int32 InSyncInterval)
{
	Delight::ThrowIfFailed(m_Swapchain->Present(InSyncInterval, 0));
}

void CRHIDirectX12::WaitForPreviousFrame()
{
	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
	// sample illustrates how to use fences for efficient resource usage and to
	// maximize GPU utilization.

	const uint64 Fence = m_fenceValue;
	Delight::ThrowIfFailed(m_CommandQueue->Signal(m_fence.GetData(), Fence));
	++m_fenceValue;

	if (m_fence->GetCompletedValue() < Fence)
	{
		Delight::ThrowIfFailed(m_fence->SetEventOnCompletion(Fence, m_fenceEvent));
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}

	FrameIndex = m_Swapchain->GetCurrentBackBufferIndex();
}

CDX12_Rendertarget& CRHIDirectX12::GetBackbuffer()
{
	return Backbuffers[FrameIndex % GNumBackbuffer];
}

Delight::Comptr<ID3D12Device> CRHIDirectX12::GetDevice()
{
	return m_Device;
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

