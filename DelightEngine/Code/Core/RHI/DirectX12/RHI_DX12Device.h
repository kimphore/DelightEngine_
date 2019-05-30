#pragma once

class CRHIDirectX12 : CRHIInterface
{
public:
	void Initialize();
private:
	void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);

private:
	ComPtr<ID3D12Device> m_Device;
	ComPtr<ID3D12CommandQueue> m_CommandQueue;
};