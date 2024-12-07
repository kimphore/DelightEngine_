#pragma once
#include "Include.h"
#include "SceneRenderer.h"

CDelightSceneRenderer::CDelightSceneRenderer()
	: bBuildedPipeline(false)
{

}

// ������ �������Ǵ� �κ�.
void CDelightSceneRenderer::Render(CDelightSceneView* SceneView)
{
	if (!bBuildedPipeline) return; // ������������ ������� ���� ������ ����.

	InitView(SceneView); // Initview�� ���������ο� ������� �����ǽ���.

	// Build�� ���������� ����.
	// ���ķ� �����Ҽ���..?
	for (auto It : currentPipeline)
	{
		It(SceneView);
	}
}

/*
	pipeline functions implementation
*/

// Culling, Visible Settings �ϴ°�.
void CDelightSceneRenderer::InitView(CDelightSceneView* SceneView)
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
