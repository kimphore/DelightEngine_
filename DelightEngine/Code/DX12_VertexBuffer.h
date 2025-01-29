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

class CRHIDirectX12;
class CDX12_VertexBuffer : public CDX12_BufferInterface
{
public:
	virtual bool8 CreateBuffer(CRHIDirectX12* RHI, EBufferType InType, uint32 InSize, void* InData = nullptr);
	virtual void SetData(void* InData, uint64 Size);

private:
	D3D12_VERTEX_BUFFER_VIEW BufferView;
};