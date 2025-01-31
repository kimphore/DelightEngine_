#pragma once
#include "Include.h"
#include "SceneRenderer.h"
#include "RHI_DX12Device.h"
#include "DX12_CommandList.h"
#include "DX12_Rendertarget.h"

CDelightSceneRenderer::CDelightSceneRenderer()
	: bBuildedPipeline(false), RHI(nullptr)
{}

void CDelightSceneRenderer::Initialize(HWND InHWnd)
{
	RHI = new CRHIDirectX12;
	if (RHI)
	{
		RHI->Initialize(InHWnd);
	}
}

void CDelightSceneRenderer::Destroy()
{
	delete RHI;
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
	RenderDX12Test(SceneView, RHI->MainCommandList);


	RHI->Present();
	RHI->WaitForPreviousFrame();	
	RHI->MainCommandList.Reset();
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

	Backbuffer.TransitionToState(&CommandList, RS_RTV);
	CommandList.Get()->OMSetRenderTargets(1, &Backbuffer.GetDescriptorCPUHandle(RT_RTV), FALSE, nullptr);

	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	CommandList.Get()->ClearRenderTargetView(Backbuffer.GetDescriptorCPUHandle(RT_RTV), clearColor, 0, nullptr);
	Backbuffer.TransitionToState(&CommandList, RS_PRESENT);
	CommandList.Close();
	CommandList.Execute(RHI->m_CommandQueue);
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
