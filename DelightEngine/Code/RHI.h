#pragma once

#include "Define.h"

class ENGINE_DLL CRHIInterface
{
public:
	virtual void Initialize(HWND hWnd) = 0;
};