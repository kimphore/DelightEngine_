#include "Include.h"
#include "EngineKernel.h"
#include "RHI_DX12Device.h"
#include "MemoryInterface.h"
#include "Logger.h"
#include "GlobalVariables.h"

#include <processthreadsapi.h>

uint64 GGameThreadFrame = 0;
uint64 GRenderThreadFrame = 0;

std::thread::id GGameThreadID;
std::thread::id GRenderThreadID;

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

void CDelightEngineKernel::InitializeThread()
{
	// 1. setup gamethread
	SetThreadDescription(GetCurrentThread(), TEXT("GameThread"));
	GGameThreadFrame = 0;
	GGameThreadID = std::this_thread::get_id();

	// 2.2 setup renderthread
}

void CDelightEngineKernel::ReleaseEngine()
{
	Delight::ShutdownLogger();
	Delight::ShutdownMemory();
}
