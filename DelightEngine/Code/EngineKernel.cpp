#include "Include.h"
#include "EngineKernel.h"
#include "RHI_DX12Device.h"
#include "MemoryInterface.h"
#include "Logger.h"
#include "GlobalVariables.h"
#include "IMGUI_GUI.h"

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

	SceneRenderer.Initialize(_hWnd);
	GUI.Initialize(_hWnd, GRHI->GetDevice(), GRHI->GetCommandQueue(), GNumBackbuffer);
	SceneRenderer.BindGUI(&GUI);

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

// 일단은 싱글쓰레드기반..
void CDelightEngineKernel::Tick_Engine()
{
	Tick_Game();
	Render();
}

void CDelightEngineKernel::Tick_Game()
{

}

LRESULT CDelightEngineKernel::WinProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (GUI.WinProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	if (Input.WinProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	return false;
}

void CDelightEngineKernel::Render()
{
	SceneRenderer.Render(nullptr);
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
