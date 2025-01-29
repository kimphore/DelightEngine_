#pragma once

#include "Include.h"

namespace Delight
{
	namespace DX12
	{
		D3D12_RESOURCE_BARRIER GetTransitionBarrier(ID3D12Resource* InResource, D3D12_RESOURCE_STATES InBeforeState, D3D12_RESOURCE_STATES InAfterState)
		{
			D3D12_RESOURCE_BARRIER Result;
			Result.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			Result.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			Result.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			Result.Transition.pResource = InResource;
			Result.Transition.StateBefore = InBeforeState;
			Result.Transition.StateAfter = InAfterState;

			return Result;
		}
	}
}