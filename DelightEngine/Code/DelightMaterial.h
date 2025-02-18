#pragma once

#include "Include.h"
#include "eastl/map.h"
#include "DelightReferenceObject.h"
#include "DelightTexture.h"

enum ETextureType
{
	TT_Diffuse,
	TT_Specular,
	TT_Emissive,
	TT_Height,
	TT_Normal,
	TT_Opacity,
	TT_AmbientOcclusion,
	TT_Basecolor,
	TT_Metalness,
	TT_Roughness,
	TT_MetalicRoughness,
};
class CDelightShader;
class CDelightTexture;
class CDelightMaterial : public CDelightReferenceObject
{
public:
	void AddTextureOnSlot(ETextureType InSlot, CDelightTexture* InTexture);
	void SetProperShader();
	virtual void InternalRelease();

private:
	eastl::map<ETextureType, Delight::SharedPtr<CDelightTexture>> TextureSlots;
	CDelightShader* MaterialShader;
};