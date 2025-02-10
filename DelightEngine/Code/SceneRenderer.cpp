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

// ������ �������Ǵ� �κ�.
void CDelightSceneRenderer::Render(CDelightSceneView* SceneView)
{
	//if (!bBuildedPipeline) return; // ������������ ������� ���� ������ ����.

	InitView(SceneView); // Initview�� ���������ο� ������� �����ǽ���.
	
	/*
	// Build�� ���������� ����.
	// ���ķ� �����Ҽ���..?
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
	RHI->EndFrame(); // ������ ������ �� ������ �ʿ��� ���(Ǯ �ʱ�ȭ ��)
}

/*
	pipeline functions implementation
*/

// Culling, Visible Settings �ϴ°�.
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

// XML���� ���� �Լ� �Ľ��Ҽ��ֵ��� �����ϴ� �� ����.
void CDelightSceneRenderer::BindPipelineFunctions()
{

}

// ������ XML�ҷ��� �����͸� ������� pipeline ����.
// ������ ������ ������������ ������ Ȯ��.
// GBuffer, Light, Postprocess, ��Ÿ �߰��Ǵ� feature��..
void CDelightSceneRenderer::BuildRenderingPipeline(tchar* pipelineName)
{

	bBuildedPipeline = true;
}
