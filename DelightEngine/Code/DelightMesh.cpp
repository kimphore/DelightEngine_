#include "DelightMesh.h"

void CDelightMesh::Initialize(FMeshInitData& InitData)
{
	Topology = InitData.Topology;
	Material = InitData.Material;
	VertexBuffer = InitData.VertexBuffer;
	IndexBuffer = InitData.IndexBuffer;

}

void CDelightMesh::InternalRelease()
{

}

