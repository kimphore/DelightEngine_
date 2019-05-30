#pragma once

class CDelightEngineKernel
{
public:
	_hres Initialize(HWND _hWnd);

private:
	HWND hWnd;

private: // Cores.
	CRHIInterface* GraphicDevice;
};