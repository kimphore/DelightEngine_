#include "Include.h"
#include "EngineKernel.h"
#include "RHI_DX12Device.h"

_result CDelightEngineKernel::Initialize(HWND _hWnd)
{
	GraphicDevice = new CRHIDirectX12;

	GraphicDevice->Initialize(_hWnd);

	return RET_SUCCES;
}

void CDelightEngineKernel::TestRenderLoop()
{
	// Just Render Test.
	GraphicDevice->m_commandAllocator->Reset();	
}
