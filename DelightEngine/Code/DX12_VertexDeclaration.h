#pragma once

#include "Include.h"
#include "comptr.h"

#include "eastl/string.h"
#include "eastl/vector.h"

/*
* Unreal Engine의 경우 시멘틱 하나당 버퍼 하나를 할당하고, InputSlot으로 여려개를 바인딩하네....
* 근데 그렇게하기엔 빡세니 우선 기본구현만
* vertexDecl.AddElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
vertexDecl.AddElement("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12);
vertexDecl.AddElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24);
*/
struct FVertexElement
{
	eastl::string SemanticName;
	uint32 SemanticIndex;
	DXGI_FORMAT Format;
	uint32 InputSlot;
	uint32 AlignedByteOffset;
	D3D12_INPUT_CLASSIFICATION InputSlotClass;
	uint32 InstanceDataStepRate;

	FVertexElement(eastl::string& InSemanticName,
		uint32 InSemanticIndex,
		DXGI_FORMAT InFormat,
		uint32 InInputSlot,
		uint32 InAlignedByteOffset,
		D3D12_INPUT_CLASSIFICATION InInputSlotClass,
		uint32 InInstanceDataStepRate)
		: SemanticName(InSemanticName), SemanticIndex(InSemanticIndex), Format(InFormat),
		InputSlot(InInputSlot), AlignedByteOffset(InAlignedByteOffset), InputSlotClass(InInputSlotClass),
		InstanceDataStepRate(InInstanceDataStepRate)
	{
	}
};

class CDX12_VertexDeclaration
{
public:
	void AddElement(FVertexElement& InElement);
	const D3D12_INPUT_LAYOUT_DESC& GetInputLayout();

private:
	eastl::vector<FVertexElement> VertexElements;
	eastl::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;
	D3D12_INPUT_LAYOUT_DESC InputLayoutDesc;

};