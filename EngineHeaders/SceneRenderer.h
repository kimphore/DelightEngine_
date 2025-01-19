#pragma once

/*!
 * \file SceneRenderer.h
 * \date 2019/08/01 14:56
 *
 * \author kimphore
 * Contact: user@company.com
 *
 * \brief Scene Renderer.
 *
 * TODO: long description
 *
 * \note
*/

class CRHIDirectX12;
class CDelightSceneView;
class CDX12_CommandList;
class ENGINE_DLL CDelightSceneRenderer
{
public:
	CDelightSceneRenderer(CRHIDirectX12* InDevice);

public:
	void Render(CDelightSceneView* SceneView);
	void InitView(CDelightSceneView* SceneView);

public: // pipeline functions.
	void RenderDX12Test(CDelightSceneView* SceneView, CDX12_CommandList& CommnadList);
public:
	void BindPipelineFunctions();
	void BuildRenderingPipeline(tchar* pipelineName);
private:
	CRHIDirectX12* RHI;

	std::map<std::wstring, std::function<void(CDelightSceneView*)>> pipelineMap;
	std::vector<std::function<void(CDelightSceneView*)>> currentPipeline;
	bool8 bBuildedPipeline;
};