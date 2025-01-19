#pragma once

#include <DirectXMath.h>
#include <d3d12.h>

#include "Typedef.h"
#include "comptr.h"

enum EBufferType
{
	Static,
	Dynamic,
	Readback,
};

class CRHIDirectX12;
class CDX12_BufferInterface
{
public:
	virtual bool8 CreateBuffer(CRHIDirectX12* RHI, uint32 InSize, EBufferType InType) = 0;
	virtual void ReleaseUploadBuffer() = 0;

public:
	D3D12_HEAP_TYPE TranslateHeapType(EBufferType InType)
	{
		switch (InType)
		{
		case EBufferType::Static:
			return D3D12_HEAP_TYPE_DEFAULT;
		case EBufferType::Dynamic:
			return D3D12_HEAP_TYPE_UPLOAD;
		case EBufferType::Readback:
			return D3D12_HEAP_TYPE_READBACK;
		}

		return D3D12_HEAP_TYPE_DEFAULT;
	}

protected:
	Delight::Comptr<ID3D12Resource> Buffer;
	Delight::Comptr<ID3D12Resource> UploadBuffer;

protected:
	EBufferType Type;
	uint32 Size;
};