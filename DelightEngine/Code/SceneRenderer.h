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

class CIMGUI_GUI;
class CRHIDirectX12;
class CDelightSceneView;
class CDX12_CommandList;
class ENGINE_DLL CDelightSceneRenderer
{
public:
	CDelightSceneRenderer();

	void Initialize(HWND InHWnd);
	void Destroy();
	void BindGUI(CIMGUI_GUI* InGUI) {
		GUI = InGUI;
	}

public:
	void Render(CDelightSceneView* SceneView);
	void RenderGUI();
	void WaitGPU();
	void InitView(CDelightSceneView* SceneView);

public: // pipeline functions.
	void RenderDX12Test(CDelightSceneView* SceneView, CDX12_CommandList& CommandList);
public:
	void BindPipelineFunctions();
	void BuildRenderingPipeline(tchar* pipelineName);
private:
	CRHIDirectX12* RHI;
	CIMGUI_GUI* GUI;

	std::map<std::wstring, std::function<void(CDelightSceneView*)>> pipelineMap;
	std::vector<std::function<void(CDelightSceneView*)>> currentPipeline;
	bool8 bBuildedPipeline;
};