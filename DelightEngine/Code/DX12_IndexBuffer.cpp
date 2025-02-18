#include "DX12_IndexBuffer.h"

#include "Include.h"
#include "RHI_DX12Device.h"
#include "DX12_ResourceUploadPool.h"

void CDX12_IndexBuffer::CreateBufferView()
{
	IBV.BufferLocation = Resource->GetGPUVirtualAddress();
	IBV.SizeInBytes = Size;
	IBV.Format = (Stride == 32) ? DXGI_FORMAT_R32_FLOAT : DXGI_FORMAT_R16_FLOAT;
}
