#pragma once
#include "Include.h"
#include "SceneRenderer.h"

CDelightSceneRenderer::CDelightSceneRenderer()
	: bBuildedPipeline(false)
{

}

// 실제로 랜더링되는 부분.
void CDelightSceneRenderer::Render(CDelightSceneView* SceneView)
{
	if (!bBuildedPipeline) return; // 파이프라인이 만들어져 있을 때에만 동작.

	InitView(SceneView); // Initview는 파이프라인에 관계없이 무조건실행.

	// Build된 파이프라인 실행.
	// 병렬로 진행할수도..?
	for (auto It : currentPipeline)
	{
		It(SceneView);
	}
}

/*
	pipeline functions implementation
*/

// Culling, Visible Settings 하는곳.
void CDelightSceneRenderer::InitView(CDelightSceneView* SceneView)
{

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
