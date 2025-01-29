#include "DX12_BufferUploadPool.h"

D3D12_HEAP_PROPERTIES CDX12_BufferUploadPool::GetUploadHeapProperties()
{
	D3D12_HEAP_PROPERTIES Result;

	Result.Type = D3D12_HEAP_TYPE_UPLOAD;
	Result.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	Result.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	Result.CreationNodeMask = 1;
	Result.VisibleNodeMask = 1;

	return Result;
}

D3D12_RESOURCE_DESC CDX12_BufferUploadPool::GetUploadHeapResourceDesc()
{
	D3D12_RESOURCE_DESC Result;

	Result.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	Result.Alignment = 0; // default 64kb
	Result.Width = GetPoolSize();
	Result.Height = 1;
	Result.DepthOrArraySize = 1;
	Result.MipLevels = 0; // auto calc
	Result.Format = DXGI_FORMAT_UNKNOWN;
	Result.SampleDesc.Count = 1;
	Result.SampleDesc.Quality = 0;
	Result.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	Result.Flags = D3D12_RESOURCE_FLAG_NONE;

	return Result;
}
