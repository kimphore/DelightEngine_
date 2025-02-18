#pragma once

#include "Include.h"

#include "DX12_Resource.h"
#include "DX12_Header.h"


enum EBufferType
{
	Static,
	Dynamic,
	Readback,
};

class CRHIDirectX12;
class CDX12_CommandList;
class CDX12_BufferInterface :public CDX12_Resource
{
public:
	CDX12_BufferInterface() {};
public:
	virtual bool8 CreateBuffer(Delight::Comptr<ID3D12Device> InDevice, EBufferType InType, uint32 InStride, uint32 InSize);
	virtual void SetData(CDX12_CommandList& CommandList, void* InData, uint64 Size);
	virtual void CreateBufferView() = 0;
	virtual EUploadPoolType GetUploadPoolType() { return UPT_VertexBuffer; }

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
	EBufferType Type;
	uint32 Size;
	uint32 Stride;
};