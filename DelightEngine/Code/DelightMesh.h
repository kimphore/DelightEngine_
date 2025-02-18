#pragma once

#include "Include.h"
#include "DelightReferenceObject.h"
#include "DX12_VertexDeclaration.h"
#include "DelightMaterial.h"

class CDX12_VertexBuffer;
class CDX12_IndexBuffer;
class CDX12_ConstantBuffer;

struct FMeshInitData
{
	D3D12_PRIMITIVE_TOPOLOGY Topology;
	Delight::SharedPtr<CDelightMaterial> Material;
	CDX12_VertexBuffer* VertexBuffer = nullptr;
	CDX12_IndexBuffer* IndexBuffer = nullptr;
};

class CDelightMesh : public CDelightReferenceObject
{
public:
	void Initialize(FMeshInitData& InitData);
public:
	virtual void InternalRelease();

protected:
	Delight::SharedPtr<CDelightMaterial> Material;

	D3D12_PRIMITIVE_TOPOLOGY Topology;
	CDX12_VertexDeclaration VertexDeclaration;
	CDX12_VertexBuffer* VertexBuffer = nullptr;
	CDX12_IndexBuffer* IndexBuffer = nullptr;
	CDX12_ConstantBuffer* ConstantBuffer = nullptr;
};