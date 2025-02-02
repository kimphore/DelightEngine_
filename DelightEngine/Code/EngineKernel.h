#pragma once

#include "Include.h"

#include "SceneRenderer.h"
#include "IMGUI_GUI.h"
#include "Win32_Input.h"

/*!
 * \file EngineKernel.h
 * \date 2019/08/01 14:06
 *
 * \author kimphore
 * Contact: user@company.com
 *
 * \brief Engine의 메인루프부분.
 *
 * TODO: long description
 *
 * \note
*/

class ENGINE_DLL CDelightEngineKernel
{
public:
	result Initialize(HWND _hWnd);
	~CDelightEngineKernel();

	void ReleaseEngine();

	void TestRenderLoop();

	void Tick_Engine();
	void Tick_Game();
	void Render();

public:
	HWND GetHWND()
	{
		return hWnd;
	}

private:
	void InitializeThread();

private:
	HWND hWnd;
	std::thread RenderThread;

private: // Cores.
	CDelightSceneRenderer SceneRenderer;
	CIMGUI_GUI GUI;
	CWin32_Input Input;
};