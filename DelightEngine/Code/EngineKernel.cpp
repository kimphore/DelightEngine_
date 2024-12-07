#include "Include.h"
#include "EngineKernel.h"
#include "RHI_DX12Device.h"
#include "MemoryInterface.h"
#include "Logger.h"
/*
* EngineKernal이 시작부...
*/
result CDelightEngineKernel::Initialize(HWND _hWnd)
{
	Delight::InitializeMemory(EMemoryType::TBBAllocation);
	Delight::InitializeLogger();

	//Delight::GraphicDevice = new CRHIDirectX12;

	//GraphicDevice->Initialize(_hWnd);

	return RET_SUCCES;
}

CDelightEngineKernel::~CDelightEngineKernel()
{
	ReleaseEngine();
}

void CDelightEngineKernel::TestRenderLoop()
{
	// Just Render Test.
	/*GraphicDevice->m_commandAllocator->Reset();	*/
}

void CDelightEngineKernel::ReleaseEngine()
{
	Delight::ShutdownLogger();
	Delight::ShutdownMemory();
}
