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
class CDX12_CommandList;
class CDX12_BufferInterface
{
public:
	virtual bool8 CreateBuffer(CRHIDirectX12* RHI, CDX12_CommandList* CommandList, EBufferType InType, uint32 InSize, void* InData = nullptr) { return false; };
	virtual void SetData(CRHIDirectX12* RHI, CDX12_CommandList* CommandList, void* InData, uint64 Size) = 0;

public:
	D3D12_HEAP_TYPE TranslateHeapType(EBufferType InType)
	{
		switch (InType)
		{
		case EBufferType::Static:
			return D3D12_HEAP_TYPE_DEFAULT;
		case EBufferType::Dynamic: // CBV, Small Vertex..
			return D3D12_HEAP_TYPE_UPLOAD;
		case EBufferType::Readback:
			return D3D12_HEAP_TYPE_READBACK;
		}

		return D3D12_HEAP_TYPE_DEFAULT;
	}

protected:
	Delight::Comptr<ID3D12Device> Device;
	Delight::Comptr<ID3D12Resource> Buffer;

protected:
	EBufferType Type;
	uint32 Size;
};