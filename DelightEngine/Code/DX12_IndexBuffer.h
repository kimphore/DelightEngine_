#pragma once

#include <DirectXMath.h>
#include <d3d12.h>

#include "DX12_Buffer.h"

#include "Typedef.h"
#include "comptr.h"

class CDX12_IndexBuffer : public CDX12_BufferInterface
{
public:
	CDX12_IndexBuffer() {};
public:
	virtual void CreateBufferView();
	virtual EUploadPoolType GetUploadPoolType() { return UPT_IndexBuffer; }
protected:
	D3D12_INDEX_BUFFER_VIEW IBV;
};