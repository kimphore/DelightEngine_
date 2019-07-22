#pragma once

class CRHIDirectX12;
class ENGINE_DLL CDelightEngineKernel
{
public:
	_result Initialize(HWND _hWnd);

	void TestRenderLoop();

private:
	HWND hWnd;

private: // Cores.
	CRHIDirectX12* GraphicDevice;
};