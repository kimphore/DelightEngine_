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
	RenderDX12Test(SceneView, RHI->MainCommandList);


	RHI->Present();
	RHI->WaitForPreviousFrame();	
	RHI->MainCommandList.Reset();
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

	Backbuffer.TransitionToState(&CommandList, RS_RTV);
	CommandList.Get()->OMSetRenderTargets(1, &Backbuffer.GetDescriptorCPUHandle(RT_RTV), FALSE, nullptr);

	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	CommandList.Get()->ClearRenderTargetView(Backbuffer.GetDescriptorCPUHandle(RT_RTV), clearColor, 0, nullptr);
	Backbuffer.TransitionToState(&CommandList, RS_PRESENT);
	CommandList.Close();
	CommandList.Execute(RHI->m_CommandQueue);
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
