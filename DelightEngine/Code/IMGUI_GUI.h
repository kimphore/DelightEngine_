#pragma once

#include "Include.h"

class CIMGUI_GUI
{
public:
	void Initialize(Delight::Comptr<ID3D12Device> InDevice, Delight::Comptr<ID3D12CommandQueue> InCommandQueue, int32 NumFrame);
};