#include "DelightTexture.h"
#include "DX12_Texture.h"

void CDelightTexture::CreateTexture(CDX12_CommandList& CommandList, eastl::string& InPath, eastl::string& InTextureName)
{

}

void CDelightTexture::InternalRelease()
{
	if (Texture)
	{
		delete Texture;
		Texture = nullptr;
	}
}