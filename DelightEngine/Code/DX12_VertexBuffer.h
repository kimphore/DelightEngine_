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
	virtual bool8 CreateBuffer(Delight::Comptr<ID3D12Device> InDevice, EBufferType InType, EVertexType InVertexType, uint32 InSize);
	virtual void SetData(CDX12_CommandList& CommandList, void* InData, uint64 Size);
	virtual uint32 GetStrideSize(EVertexType InVertexType);
protected:
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView;
	EVertexType VertexType;
};