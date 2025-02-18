#include "DelightMaterial.h"

void CDelightMaterial::AddTextureOnSlot(ETextureType InSlot, CDelightTexture* InTexture)
{
	TextureSlots.insert(eastl::make_pair(InSlot, InTexture));
}

void CDelightMaterial::SetProperShader()
{
	// material은 동일, TextureType에 대한 변형만 적용
}

void CDelightMaterial::InternalRelease()
{

}

