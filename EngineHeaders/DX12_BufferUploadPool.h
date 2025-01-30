#pragma once
#include "DX12_ResourceUploadPool.h"

// 32mb
#define BUFFER_POOL_SIZE 32 * 1024 * 1024

class CRHIDirectX12;
class CDX12_BufferInterface;
class CDX12_CommandList;

class CDX12_BufferUploadPool : public CDX12_ResourceUpdatePool
{
public:
	virtual uint64 GetPoolSize() {
		return BUFFER_POOL_SIZE;
	}

protected:
	virtual D3D12_HEAP_PROPERTIES GetUploadHeapProperties();
	virtual D3D12_RESOURCE_DESC GetUploadHeapResourceDesc();
};