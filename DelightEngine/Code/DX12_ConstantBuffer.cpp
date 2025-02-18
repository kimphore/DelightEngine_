#include "DX12_ConstantBuffer.h"

#include "Include.h"
#include "RHI_DX12Device.h"
#include "DX12_ResourceUploadPool.h"
#include "DX12_DescriptorHeapManager.h"

void CDX12_ConstantBuffer::CreateBufferView()
{
	CBV.BufferLocation = Resource->GetGPUVirtualAddress();
	// constant buffer는 256byte align으로 접근해야 함.
	CBV.SizeInBytes = (Size + 255) & ~255;
	CBVHandle = GDescriptorHeapManager.GetHandle(DHT_ShaderResource);

	Device->CreateConstantBufferView(&CBV, CBVHandle.CPUHandle);
}

void CDX12_ConstantBuffer::SetCBVOnRootSigniture(CDX12_CommandList& Commandlist)
{
	//commandList->SetGraphicsRootDescriptorTable(0, gpuCbvHandle);
}
