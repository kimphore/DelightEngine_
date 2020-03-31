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

class CDelightSceneView;
class ENGINE_DLL CDelightSceneRenderer
{
public:
	CDelightSceneRenderer();

public:
	void Render(CDelightSceneView* SceneView);
	void InitView(CDelightSceneView* SceneView);

public: // pipeline functions.

public:
	void BindPipelineFunctions();
	void BuildRenderingPipeline(Tchar* pipelineName);
private:
	std::map<std::wstring, std::function<void(CDelightSceneView*)>> pipelineMap;
	std::vector<std::function<void(CDelightSceneView*)>> currentPipeline;
	Bool8 bBuildedPipeline;
};