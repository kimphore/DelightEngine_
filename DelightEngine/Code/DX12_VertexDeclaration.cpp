#include "DX12_VertexDeclaration.h"

void CDX12_VertexDeclaration::AddElement(FVertexElement& InElement)
{
	VertexElements.push_back(InElement);
}

const D3D12_INPUT_LAYOUT_DESC& CDX12_VertexDeclaration::GetInputLayout()
{
	InputElementDesc.clear();
	for (const auto e : VertexElements)
	{
		InputElementDesc.push_back(
			{
				e.SemanticName.c_str(),
				e.SemanticIndex,
				e.Format,
				e.InputSlot,
				e.AlignedByteOffset,
				e.InputSlotClass,
				e.InstanceDataStepRate
			});

		InputLayoutDesc.NumElements = (uint32)InputElementDesc.size();
		InputLayoutDesc.pInputElementDescs = InputElementDesc.data();
	}

	return InputLayoutDesc;
}

