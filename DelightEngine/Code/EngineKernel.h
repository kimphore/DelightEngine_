#pragma once

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

class CRHIDirectX12;
class ENGINE_DLL CDelightEngineKernel
{
public:
	result Initialize(HWND _hWnd);
	~CDelightEngineKernel();

	void ReleaseEngine();

	void TestRenderLoop();

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
	CRHIDirectX12* GraphicDevice;
};