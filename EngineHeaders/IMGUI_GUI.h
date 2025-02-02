#pragma once

#include "Include.h"

#include "DX12_CommandList.h"

class CIMGUI_GUI
{
public:
	void Initialize(HWND InHWnd, Delight::Comptr<ID3D12Device> InDevice, Delight::Comptr<ID3D12CommandQueue> InCommandQueue, int32 NumFrame);
	void Release();
	
	// 나중에 Resize시 재생성...고려
	void Resize() {};

	void Render(CDX12_CommandList& CommandList);

public:
	LRESULT WinProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private: // windows
	void BuildFramerateWindow();

private:
	Delight::Comptr<ID3D12Device> Device;
	Delight::Comptr<ID3D12CommandQueue> CommandQueue;
	HWND myHWnd;
	bool8 bShowWindow = true;
};