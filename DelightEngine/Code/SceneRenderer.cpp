#pragma once
#include "Include.h"
#include "SceneRenderer.h"
#include "RHI_DX12Device.h"
#include "DX12_CommandList.h"

CDelightSceneRenderer::CDelightSceneRenderer(CRHIDirectX12* InDevice)
	: bBuildedPipeline(false),
	RHI(InDevice)
{

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
	CDX12_CommandList CommandList;
	CommandList.Init(RHI);
	RenderDX12Test(SceneView, CommandList);
	
}

/*
	pipeline functions implementation
*/

// Culling, Visible Settings �ϴ°�.
void CDelightSceneRenderer::InitView(CDelightSceneView* SceneView)
{

}

void CDelightSceneRenderer::RenderDX12Test(CDelightSceneView* SceneView, CDX12_CommandList& CommnadList)
{

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
