#pragma once

#include "Include.h"
#include "EngineKernel.h"

#include "eastl/vector.h"
#include "eastl/string.h"

#include "DX12_CommandList.h"

enum ESceneType
{
	ST_PBR,
	ST_NonPBR
};

class CDelightTexture;
class CDelightMesh;
class CDelightMaterial;
struct aiScene;
struct aiMaterial;
class ENGINE_DLL CDelightAssimpImporter
{
public:
	void LoadScene(std::string& InPath, std::string& FileName, ESceneType InSceneType, CDelightEngineKernel* InEngineKernel);

private:
	void LoadMaterialAndTexture(CDX12_CommandList& CommandList, const aiScene* InScene);
	void LoadMesh(CDX12_CommandList& CommandList, const aiScene* InScene);
	void LoadComponent(const aiScene* InScene);
	void Finalize(CDelightEngineKernel* InEngineKernel);

private:
	void LoadTextureForMaterial(CDX12_CommandList& CommandList, aiMaterial* InAssimpMat, CDelightMaterial* InMat, int32 Type, int32 MaterialSlot);

private:
	ESceneType SceneType;
	eastl::string Path;
	eastl::vector<CDelightTexture*> TextureArray;
	eastl::vector<CDelightMesh*> MeshArray;
	eastl::vector<CDelightMaterial*> MaterialArray;
};