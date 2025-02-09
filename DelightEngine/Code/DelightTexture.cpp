#include "DelightTexture.h"

#include "DirectXLibs/d3dx12.h"
#include "DirectXTex.h"

#include "Function.h"
#include "DX12_Functions.h"
#include "RHI_DX12Device.h"

void CDelightTexture::CreateTexture(CDX12_CommandList& CommandList, eastl::string& InPath, eastl::string& InTextureName)
{
	eastl::basic_string<TCHAR> ConvertedString;

	Delight::ConvertCharToTChar(InPath, ConvertedString);
	TextureName = InTextureName;
	Device = GRHI->GetDevice();

	DirectX::ScratchImage Image;
	Delight::ThrowIfFailed(DirectX::LoadFromWICFile(ConvertedString.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, Image));

	const DirectX::Image* img = Image.GetImages();
	Delight::ThrowIfFailed(DirectX::CreateTexture(Device.GetData(), Image.GetMetadata(), &Resource));

	// 풀쓰지말자 머리아프다..
	uint64 uploadBufferSize = GetRequiredIntermediateSize(Resource.GetData(), 0, 1);
	D3D12_HEAP_PROPERTIES HeapProperty = {D3D12_HEAP_TYPE_UPLOAD};
	D3D12_RESOURCE_DESC BufferDesc = {};

	BufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	BufferDesc.Width = uploadBufferSize;
	BufferDesc.Height = 1;
	BufferDesc.DepthOrArraySize = 1;
	BufferDesc.MipLevels = 1;
	BufferDesc.Format = DXGI_FORMAT_UNKNOWN;
	BufferDesc.SampleDesc.Count = 1;
	BufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	Delight::ThrowIfFailed(Device->CreateCommittedResource(&HeapProperty, D3D12_HEAP_FLAG_NONE, &BufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, DELIGHT_IID_PPV_ARGS(&UploadBuffer)));

	D3D12_SUBRESOURCE_DATA SubresourceData;
	SubresourceData.pData = img->pixels;
	SubresourceData.RowPitch = img->rowPitch;
	SubresourceData.SlicePitch = img->slicePitch;

	UpdateSubresources(CommandList.Get().GetData(), Resource.GetData(), UploadBuffer.GetData(), 0, 0, 1, &SubresourceData);

	D3D12_RESOURCE_BARRIER ResourceBarrier = Delight::DX12::GetTransitionBarrier(Resource.GetData(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	CommandList->ResourceBarrier(1, &ResourceBarrier);
}

void CDelightTexture::Release()
{

}

