#pragma once

#include <DirectXMath.h>
#include <d3d12.h>

#include "DX12_Buffer.h"

#include "Typedef.h"
#include "comptr.h"

struct FPositionOnlyVertex
{
	DirectX::XMFLOAT3 Position;
};

enum EVertexType
{
	PositionOnly,
};

class CRHIDirectX12;
class CDX12_VertexBuffer : public CDX12_BufferInterface
{
public:
	CDX12_VertexBuffer() {};
public:
	virtual void CreateBufferView();
	virtual EUploadPoolType GetUploadPoolType() { return UPT_VertexBuffer; }

protected:
	D3D12_VERTEX_BUFFER_VIEW VBV;
	EVertexType VertexType;
};