#include "DelightMaterial.h"

void CDelightMaterial::AddTextureOnSlot(ETextureType InSlot, CDelightTexture* InTexture)
{
	TextureSlots.insert(eastl::make_pair(InSlot, InTexture));
}

void CDelightMaterial::SetProperShader()
{
	// material�� ����, TextureType�� ���� ������ ����
}

void CDelightMaterial::InternalRelease()
{

}

