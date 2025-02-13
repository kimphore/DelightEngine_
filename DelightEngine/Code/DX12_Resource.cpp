#include "DX12_Resource.h"
#include "DX12_Functions.h"

void CDX12_Resource::TransitionState(CDX12_CommandList& CommandList, D3D12_RESOURCE_STATES NextState)
{
	if (ResourceState != NextState)
	{
		D3D12_RESOURCE_BARRIER Barrier = Delight::DX12::GetTransitionBarrier(Resource.GetData(),
			ResourceState, NextState);

		CommandList->ResourceBarrier(1, &Barrier);
		ResourceState = NextState;
	}
}