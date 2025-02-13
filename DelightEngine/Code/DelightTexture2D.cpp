#include "DelightTexture2D.h"
#include "DX12_Texture2D.h"


void CDelightTexture2D::CreateTexture(CDX12_CommandList& CommandList, eastl::string& InPath, eastl::string& InTextureName)
{
	Texture = new CDX12_Texture2D;
	if (Texture)
	{
		Texture->CreateTexture(CommandList, InPath, InTextureName);
	}
}

void CDelightTexture2D::Release()
{
	delete Texture;
}

