#include "include.h"
#include "RHI_DX12Device.h"

void CRHIDirectX12::Initialize()
{
	UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
	ComPtr<ID3D12Debug> debugController;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	}
#endif

	ComPtr<IDXGIFactory4> factory;
	Delight::FailedReturnString(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory),
		TEXT("Failed Create DXGI Factory.")));

	ComPtr<IDXGIAdapter1> hardwareAdapter;
	
	GetHardwareAdapter(factory.Get(), &hardwareAdapter);

	Delight::FailedReturnString(D3D12CreateDevice(hardwareAdapter.Get(), 
		D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)), 
		TEXT("Failed Create D3D12 Device"));

	// Create Command Queue.

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flag = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	Delight::FailedReturnString(m_Device->CreateCommnadQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)),
		TEXT("Failed Create Command Queue."));

	// Creat SwapChain.
	DXGI_SWAP_CHAIN_DESC1 chainDesc;
	chainDesc.BufferCount = 2; // double buffering.
	chainDesc.Width = 1280;
	chainDesc.Height = 720;
	chainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.SampleDesc.Count = 1; // multisampling off..?

	ComPtr<IDXGISwapChain1> swapChain;
}

void CRHIDirectX12::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;	
	*ppAdapter = nullptr;

	for (_uint index = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(index, &adapter); ++index)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// can't use hardware adapter..
			continue;
		}

		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))
		{
			break;
		}
	}
	
	*ppAdapter = adapter.Detach();
}

