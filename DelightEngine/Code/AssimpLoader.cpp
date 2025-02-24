#include "AssimpLoader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "DelightActor.h"
#include "DelightTexture2D.h"
#include "DelightMaterial.h"
#include "DelightMesh.h"

#include "SceneRenderer.h"
#include "RHI_DX12Device.h"

#include "DX12_CommandListPool.h"
#include "DX12_Fence.h"
#include "DX12_CommandList.h"

#include "DX12_VertexBuffer.h"
#include "DX12_IndexBuffer.h"
#include "DX12_ResourceUploadPool.h"

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
	LoadActor(Scene);

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
	MeshArray.reserve(InScene->mNumMeshes);
	for (uint32 MeshIndex = 0; MeshIndex < InScene->mNumMeshes; ++MeshIndex)
	{
		aiMesh* Mesh = InScene->mMeshes[MeshIndex];

		uint32 VertexNum = Mesh->mNumVertices;
		eastl::vector<FDefaultVertex> Vertices;
		for (uint32 VertexIndex = 0; VertexIndex < VertexNum; ++VertexIndex)
		{
			FDefaultVertex Temp;

			Temp.Position = Vector3(Mesh->mVertices[VertexIndex].x, Mesh->mVertices[VertexIndex].y, Mesh->mVertices[VertexIndex].z);
			Temp.Normal = Vector3(Mesh->mNormals[VertexIndex].x, Mesh->mNormals[VertexIndex].y, Mesh->mNormals[VertexIndex].z);
			Temp.Tangent = Vector3(Mesh->mTangents[VertexIndex].x, Mesh->mTangents[VertexIndex].y, Mesh->mTangents[VertexIndex].z);
			Temp.UV = Vector2(Mesh->mTextureCoords[0][VertexIndex].x, Mesh->mTextureCoords[0][VertexIndex].y);
			
			Vertices.push_back(Temp);
		}

		uint32 FaceNum = Mesh->mNumFaces;
		bool8 bNeed32Bit = FaceNum * 3 >= UINT16_MAX;
		
		eastl::vector<uint32> Indices32Bit;
		eastl::vector<uint16> Indices16Bit;

		if (bNeed32Bit)
		{
			Indices32Bit.reserve(FaceNum * 3);
		}
		else
		{
			Indices16Bit.reserve(FaceNum * 3);
		}

		for (uint32 FaceIndex = 0; FaceIndex < FaceNum; ++FaceIndex)
		{
			aiFace& Face = Mesh->mFaces[FaceIndex];
			for (uint32 i = 0; i < Face.mNumIndices; ++i)
			{
				if (bNeed32Bit)
				{
					Indices32Bit.push_back(Face.mIndices[i]);
				}
				else
				{
					Indices16Bit.push_back(Face.mIndices[i]);
				}
			}
		}
		/*
		* CreateBuffers.
		*/
		CDX12_VertexBuffer* VertexBuffer = new CDX12_VertexBuffer;
		CDX12_IndexBuffer* IndexBuffer = new CDX12_IndexBuffer;
		uint32 VertexBufferSize = GDefaultVertexStride * (uint32)Vertices.size();
		uint32 IndexStride = bNeed32Bit ? 8 : 4;
		uint32 IndexBufferSize = IndexStride * (uint32)Indices16Bit.size();

		VertexBuffer->CreateBuffer(GRHI->GetDevice(), EBufferType::Static, GDefaultVertexStride, VertexBufferSize);
		VertexBuffer->SetData(CommandList, (void*)Vertices.data(), VertexBufferSize);

		IndexBuffer->CreateBuffer(GRHI->GetDevice(), EBufferType::Static, IndexStride, IndexBufferSize);
		IndexBuffer->SetData(CommandList, bNeed32Bit ? (void*)Indices32Bit.data() : (void*)Indices16Bit.data(), IndexBufferSize);

		FMeshInitData InitData;
		InitData.Material = MaterialArray[Mesh->mMaterialIndex];
		InitData.Topology = (D3D12_PRIMITIVE_TOPOLOGY)Mesh->mPrimitiveTypes;
		InitData.VertexBuffer = VertexBuffer;
		InitData.IndexBuffer = IndexBuffer;

		CDelightMesh* NewMesh = new CDelightMesh;
		NewMesh->Initialize(InitData);

		MeshArray.push_back(NewMesh);
	}

	FlushAndWaitResourcePoolUpload(CommandList);
}

void CDelightAssimpImporter::LoadActor(const aiScene* InScene)
{
	if (InScene->mRootNode)
	{
		InternalLoadActor(InScene->mRootNode, InScene->mRootNode->mTransformation);
	}
}

void CDelightAssimpImporter::Finalize(CDelightEngineKernel* InEngineKernel)
{

}

void CDelightAssimpImporter::InternalLoadActor(aiNode* ParentNode, aiMatrix4x4& ParentMatrix)
{
	if (ParentNode)
	{
		if (ParentNode->mNumMeshes)
		{
			for (int32 i = 0; i < ParentNode->mNumMeshes; ++i)
			{
				/*
				* CreateActors.
				*/
				CDelightActor* NewActor = new CDelightActor;
			}
		}

		if (ParentNode->mNumChildren > 0)
		{
			for (int32 i = 0; i < ParentNode->mNumChildren; ++i)
			{
				aiMatrix4x4 CurrentMatrix = ParentMatrix * ParentNode->mChildren[i]->mTransformation;
				InternalLoadActor(ParentNode->mChildren[i], CurrentMatrix);
			}
		}
	}
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