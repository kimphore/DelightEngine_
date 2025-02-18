#include "DX12_VertexBuffer.h"

#include "Include.h"
#include "RHI_DX12Device.h"
#include "DX12_ResourceUploadPool.h"

void CDX12_VertexBuffer::CreateBufferView()
{
	VBV.BufferLocation = Resource->GetGPUVirtualAddress();
	VBV.StrideInBytes = Stride;
	VBV.SizeInBytes = Size;
}
