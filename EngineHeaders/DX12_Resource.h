#pragma once

#include "Include.h"
#include "comptr.h"

#include "DX12_CommandList.h"
#include "eastl/string.h"

class CDX12_Resource
{
public:
	virtual void DeleteUploadBuffer() {}
public:
	void TransitionState(CDX12_CommandList& CommandList, D3D12_RESOURCE_STATES NextState);
	
public:
	// initialize¶§¸¸
	void SetResourceState(D3D12_RESOURCE_STATES InState) {
		ResourceState = InState;
	}

	D3D12_RESOURCE_STATES GetResourceState() {
		return ResourceState;
	}

	Delight::Comptr<ID3D12Resource> GetResource() {
		return Resource;
	}

	Delight::Comptr<ID3D12Device> GetDevice() {
		return Device;
	}

	void SetResourceName(eastl::string& InName){
		Name = InName;
	}

	eastl::string& GetName(){
		return Name;
	}

	void SetDevice(Delight::Comptr<ID3D12Device> InDevice) {
		Device = InDevice;
	}

protected:
	Delight::Comptr<ID3D12Resource> Resource;
	Delight::Comptr<ID3D12Device> Device;
	D3D12_RESOURCE_STATES ResourceState; // current state
	eastl::string Name;
};