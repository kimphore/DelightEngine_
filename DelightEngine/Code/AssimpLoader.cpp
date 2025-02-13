#include "AssimpLoader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "DelightTexture2D.h"
#include "DelightMaterial.h"
#include "DelightMesh.h"

#include "SceneRenderer.h"
#include "RHI_DX12Device.h"

#include "DX12_CommandListPool.h"
#include "DX12_Fence.h"
#include "DX12_CommandList.h"

/*
*	Actor
*		- Mesh
*			- Material
*				- Texture..
*/

void CDelightAssimpImporter::LoadScene(std::string& InPath, std::string& FileName, ESceneType InSceneType, CDelightEngineKernel* InEngineKernel)
{
	Assimp::Importer Importer;

	uint32 Flags = aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_CalcTangentSpace |
		aiProcess_GenNormals |
		aiProcess_MakeLeftHanded |
		aiProcess_FlipWindingOrder;

	Path = InPath.c_str();
	SceneType = InSceneType;

	std::string FullPath = InPath + FileName;

	const aiScene* Scene = Importer.ReadFile(FullPath, Flags);

	if (Scene == nullptr)
	{
		return;
	}

	Delight::Comptr<ID3D12CommandQueue> CommandQueue = GRHI->GetCommandQueue();
	CDX12_Fence ResourceFence;
	CDX12_CommandList CommandList = GCommandListPool.GetCommandList();
	ResourceFence.Initialize(CommandList.GetDevice());
	LoadMaterialAndTexture(CommandList, Scene);

	CommandList.Close();
	CommandList.Execute(CommandQueue);

	CommandList = GCommandListPool.GetCommandList();

	LoadMesh(CommandList, Scene);
	LoadComponent(Scene);

	ResourceFence.Wait(CommandQueue);

	Finalize(InEngineKernel);
}

/*
* fbx파일은 pbr정보를 정상적으로 저장못함.
* gltf포멧사용할것.
*/
void CDelightAssimpImporter::LoadMaterialAndTexture(CDX12_CommandList& CommandList, const aiScene* InScene)
{
	MaterialArray.resize(InScene->mNumMaterials);

	for (uint32 MaterialIndex = 0; MaterialIndex < InScene->mNumMaterials; ++MaterialIndex)
	{
		CDelightMaterial* Material = new CDelightMaterial;
		aiMaterial*& aiMat = InScene->mMaterials[MaterialIndex];
		if (Material && aiMat)
		{
			LoadTextureForMaterial(CommandList, aiMat, Material, aiTextureType_BASE_COLOR, TT_Basecolor);
			LoadTextureForMaterial(CommandList, aiMat, Material, aiTextureType_NORMALS, TT_Normal);
			LoadTextureForMaterial(CommandList, aiMat, Material, aiTextureType_METALNESS, TT_Metalness);
			LoadTextureForMaterial(CommandList, aiMat, Material, aiTextureType_GLTF_METALLIC_ROUGHNESS, TT_MetalicRoughness);
			Material->SetProperShader();
			MaterialArray[MaterialIndex] = Material;
		}
		else
		{
			MaterialArray[MaterialIndex] = nullptr;
		}
	}
}

void CDelightAssimpImporter::LoadMesh(CDX12_CommandList& CommandList, const aiScene* InScene)
{

}

void CDelightAssimpImporter::LoadComponent(const aiScene* InScene)
{

}

void CDelightAssimpImporter::Finalize(CDelightEngineKernel* InEngineKernel)
{

}

void CDelightAssimpImporter::LoadTextureForMaterial(CDX12_CommandList& CommandList, aiMaterial* InAssimpMat, CDelightMaterial* InMat, int32 Type, int32 MaterialSlot)
{
	CDelightTexture* NewTexture = new CDelightTexture2D;
	aiTextureType ImportType = aiTextureType(Type);
	ETextureType ExportType = ETextureType(MaterialSlot);
	eastl::string FullPath, AssetName;

	aiString TexturePath;
	if (InAssimpMat->GetTexture(ImportType, 0, &TexturePath) == AI_SUCCESS)
	{
		AssetName = TexturePath.data;
		FullPath = Path + AssetName;
		// init
		NewTexture->CreateTexture(CommandList, FullPath, AssetName);
		InMat->AddTextureOnSlot(ExportType, NewTexture);
	}	
}