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
	Result Initialize(HWND _hWnd);

	void TestRenderLoop();

private:
	HWND hWnd;

private: // Cores.
	CRHIDirectX12* GraphicDevice;
};