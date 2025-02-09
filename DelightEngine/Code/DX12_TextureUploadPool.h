#pragma once
#include "DX12_ResourceUploadPool.h"

// 64mb
#define TEXTURE_POOL_SIZE 64 * 1024 * 1024

class CRHIDirectX12;
class CDX12_BufferInterface;
class CDX12_CommandList;

class CDX12_TextureUploadPool : public CDX12_ResourceUpdatePool
{
public:
	virtual uint64 GetPoolSize() {
		return TEXTURE_POOL_SIZE;
	}

protected:
	void InternalUploadData(CDX12_CommandList& CommandList, Delight::Comptr<ID3D12Resource> Dest, FResourceUploadData& InData);

protected:
	virtual D3D12_HEAP_PROPERTIES GetUploadHeapProperties();
	virtual D3D12_RESOURCE_DESC GetUploadHeapResourceDesc();
};