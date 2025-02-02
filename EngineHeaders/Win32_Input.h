#pragma once

#include "Include.h"

class CWin32_Input
{
public:
	LRESULT WinProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};