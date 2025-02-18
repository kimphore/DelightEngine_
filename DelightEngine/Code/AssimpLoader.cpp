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
	MeshArray.reserve(InScene->mNumMeshes);
	for (uint32 MeshIndex = 0; MeshIndex < InScene->mNumMeshes; ++MeshIndex)
	{
		/*
		* D3D_PRIMITIVE_TOPOLOGY 
		* 		mPrimitiveTypes	4	unsigned int
		mNumVertices	3159	unsigned int
		mNumFaces	3640	unsigned int
+		mVertices	0x0000017643291040 {x=4.76183176 y=1.28588295 z=-2.24458694 }	aiVector3t<float> *
+		mNormals	0x0000017641dfeb40 {x=-0.651900887 y=0.0427000597 z=-0.757101119 }	aiVector3t<float> *
+		mTangents	0x0000017641e07f90 {x=0.289943635 y=-0.908512414 z=-0.300895095 }	aiVector3t<float> *
+		mBitangents	0x00000176465378d0 {x=0.647832870 y=0.550287008 z=-0.526779771 }	aiVector3t<float> *
+		mColors	0x00000176419e3100 {0x0000000000000000 <NULL>, 0x0000000000000000 <NULL>, 0x0000000000000000 <NULL>, ...}	aiColor4t<float> *[8]
+		mTextureCoords	0x00000176419e3140 {0x0000017641e113e0 {x=0.609000027 y=0.667599976 z=0.00000000 }, 0x0000000000000000 <NULL>, ...}	aiVector3t<float> *[8]
+		mNumUVComponents	0x00000176419e3180 {2, 0, 0, 0, 0, 0, 0, 0}	unsigned int[8]
+		mFaces	0x0000017641e487f8 {mNumIndices=3 mIndices=0x0000017641d4d020 {2} }	aiFace *
		mNumBones	0	unsigned int
+		mBones	0x0000000000000000 {???}	aiBone * *
		mMaterialIndex	0	unsigned int
+		mName	{length=9 data=0x00000176419e31c0 "meshes[0]" }	aiString
mPrimitiveTypes
		*/
		aiMesh* Mesh = InScene->mMeshes[MeshIndex];

		uint32 VertexNum = Mesh->mNumVertices;
		eastl::vector<FDefaultVertex> Vertices;
		for (uint32 VertexIndex = 0; VertexIndex < VertexNum; ++VertexIndex)
		{
			FDefaultVertex Temp;

			Temp.Position = FFloat3(Mesh->mVertices[VertexIndex].x, Mesh->mVertices[VertexIndex].y, Mesh->mVertices[VertexIndex].z);
			Temp.Normal = FFloat3(Mesh->mNormals[VertexIndex].x, Mesh->mNormals[VertexIndex].y, Mesh->mNormals[VertexIndex].z);
			Temp.Tangent = FFloat3(Mesh->mTangents[VertexIndex].x, Mesh->mTangents[VertexIndex].y, Mesh->mTangents[VertexIndex].z);
			Temp.UV = FFloat2(Mesh->mTextureCoords[0][VertexIndex].x, Mesh->mTextureCoords[0][VertexIndex].y);
			
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