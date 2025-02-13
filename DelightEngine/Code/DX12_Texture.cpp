#include "DX12_Texture.h"
#include "DirectXLibs/d3dx12.h"
#include "DirectXTex.h"

#include "Function.h"
#include "DX12_Functions.h"
#include "RHI_DX12Device.h"


void CDX12_Texture::CreateTexture(CDX12_CommandList& CommandList, FTextureCreateDesc& InDesc)
{
	if (GRHI == nullptr)
	{
		return;
	}

	SetDevice(GRHI->GetDevice());
	SetResourceName(InDesc.ResourceName.length() > 0 ? InDesc.ResourceName : "Texture2D");

	D3D12_RESOURCE_DESC ResourceDesc = {};
	ResourceDesc.Dimension = InDesc.SizeZ > 1 ? D3D12_RESOURCE_DIMENSION_TEXTURE3D : D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.Width = InDesc.SizeX;
	ResourceDesc.Height = InDesc.SizeY;
	ResourceDesc.DepthOrArraySize = InDesc.SizeZ;
	ResourceDesc.MipLevels = InDesc.MipLevels;
	ResourceDesc.Format = InDesc.Format;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	ResourceDesc.Flags = InDesc.Flags;

	D3D12_HEAP_PROPERTIES heapProps = { D3D12_HEAP_TYPE_DEFAULT };

	SetResourceState(D3D12_RESOURCE_STATE_COPY_DEST);

	Device->CreateCommittedResource(&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		GetResourceState(),
		nullptr,
		DELIGHT_IID_PPV_ARGS(&Resource)
	);
}

void CDX12_Texture::DeleteUploadBuffer()
{
	ID3D12Resource* Temp = UploadBuffer.Detach();
	Temp->Release();
}

