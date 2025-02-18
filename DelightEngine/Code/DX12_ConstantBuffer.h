#pragma once

#include "DX12_Buffer.h"
#include "DX12_DescriptorHeapManager.h"

class CDX12_ConstantBuffer : public CDX12_BufferInterface
{
public:
	CDX12_ConstantBuffer() {};
public:
	virtual void CreateBufferView();
	virtual EUploadPoolType GetUploadPoolType() { return UPT_VertexBuffer; }

public:
	// get handle?
	/*
		RootSigniture 기반 : CPU
		Descriptor heap 기반 : GPU
	*/
	void SetCBVOnRootSigniture(CDX12_CommandList& Commandlist);
protected:
	D3D12_CONSTANT_BUFFER_VIEW_DESC CBV;
	FDescriptorHandleSet CBVHandle;
};