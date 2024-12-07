#pragma once

/*!
 * \file EngineKernel.h
 * \date 2019/08/01 14:06
 *
 * \author kimphore
 * Contact: user@company.com
 *
 * \brief Engine�� ���η����κ�.
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

	void TestRenderLoop();

private:
	void ReleaseEngine();

private:
	HWND hWnd;

private: // Cores.
	CRHIDirectX12* GraphicDevice;
};