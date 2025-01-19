#pragma once

#include <DirectXMath.h>
#include <d3d12.h>

#include "Typedef.h"
#include "comptr.h"

struct FPositionOnlyVertex
{
	DirectX::XMFLOAT3 Position;
};

enum EVertexBufferType
{
	Static,
	Dynamic,
	Readback,
};

class CRHIDirectX12;
class CDX12_VertexBuffer
{
public:
	void CreateBuffer(CRHIDirectX12* RHI, uint32 Size, EVertexBufferType InType);
	
	D3D12_HEAP_TYPE TranslateHeapType(EVertexBufferType InType);

private:
	Delight::Comptr<ID3D12Resource> Buffer;
};