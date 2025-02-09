#pragma once
#include "Include.h"
#include "SceneRenderer.h"
#include "RHI_DX12Device.h"
#include "DX12_CommandList.h"
#include "DX12_Rendertarget.h"
#include "DX12_CommandListPool.h"
#include "IMGUI_GUI.h"

CDelightSceneRenderer::CDelightSceneRenderer()
	: bBuildedPipeline(false), RHI(nullptr)
{}

void CDelightSceneRenderer::Initialize(HWND InHWnd)
{
	RHI = new CRHIDirectX12;
	if (RHI)
	{
		RHI->Initialize(InHWnd);
		GRHI = RHI;
	}

	myHWnd = InHWnd;
}

void CDelightSceneRenderer::Destroy()
{
	delete RHI;
}

void CDelightSceneRenderer::BindGUI(CIMGUI_GUI* InGUI)
{
	GUI = InGUI;
}

// 실제로 랜더링되는 부분.
void CDelightSceneRenderer::Render(CDelightSceneView* SceneView)
{
	//if (!bBuildedPipeline) return; // 파이프라인이 만들어져 있을 때에만 동작.

	InitView(SceneView); // Initview는 파이프라인에 관계없이 무조건실행.
	
	/*
	// Build된 파이프라인 실행.
	// 병렬로 진행할수도..?
	for (auto It : currentPipeline)
	{
		It(SceneView);
	}
	*/
	extern CDX12_CommandListPool GCommandListPool;
	CDX12_CommandList CommandList;

	GCommandListPool.GetCommandList(CommandList);
	CommandList.Reset();

	RenderDX12Test(SceneView, CommandList);

	RenderGUI();

	WaitGPU();
}

void CDelightSceneRenderer::RenderGUI()
{
	if (GUI)
	{
		CDX12_Rendertarget& Backbuffer = RHI->GetBackbuffer();
		CDX12_CommandList CommandList = GCommandListPool.GetCommandList();
		CommandList.Reset();

		RHI->BindDescriptionHeaps(CommandList);

		Backbuffer.TransitionToState(&CommandList, RS_RTV);
		CommandList.Get()->OMSetRenderTargets(1, &Backbuffer.GetDescriptorCPUHandle(RT_RTV), FALSE, nullptr);

		GUI->Render(CommandList);

		Backbuffer.TransitionToState(&CommandList, RS_PRESENT);
		CommandList.Close();
		CommandList.Execute(RHI->GetCommandQueue());
	}
}

void CDelightSceneRenderer::WaitGPU()
{
	RHI->Present();
	RHI->WaitForPreviousFrame();
	RHI->EndFrame(); // 프레임 끝났을 때 실행이 필요한 썸띵(풀 초기화 등)
}

/*
	pipeline functions implementation
*/

// Culling, Visible Settings 하는곳.
void CDelightSceneRenderer::InitView(CDelightSceneView* SceneView)
{

}

void CDelightSceneRenderer::RenderDX12Test(CDelightSceneView* SceneView, CDX12_CommandList& CommandList)
{
	CDX12_Rendertarget& Backbuffer = RHI->GetBackbuffer();

	RHI->BindDescriptionHeaps(CommandList);

	Backbuffer.TransitionToState(&CommandList, RS_RTV);
	CommandList.Get()->OMSetRenderTargets(1, &Backbuffer.GetDescriptorCPUHandle(RT_RTV), FALSE, nullptr);

	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	CommandList.Get()->ClearRenderTargetView(Backbuffer.GetDescriptorCPUHandle(RT_RTV), clearColor, 0, nullptr);
	Backbuffer.TransitionToState(&CommandList, RS_PRESENT);
	CommandList.Close();
	CommandList.Execute(RHI->GetCommandQueue());
}

// XML에서 각각 함수 파싱할수있도록 대응하는 맵 구성.
void CDelightSceneRenderer::BindPipelineFunctions()
{

}

// 실제로 XML불러온 데이터를 기반으로 pipeline 구성.
// 목적은 랜더링 파이프라인의 유연성 확보.
// GBuffer, Light, Postprocess, 기타 추가되는 feature들..
void CDelightSceneRenderer::BuildRenderingPipeline(tchar* pipelineName)
{

	bBuildedPipeline = true;
}
