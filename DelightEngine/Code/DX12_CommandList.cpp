#include "DX12_CommandList.h"
#include "Include.h"
#include "Exception.h"
#include "CriticalSection.h"
#include "RHI_DX12Device.h"

void CDX12_CommandList::Init(CRHIDirectX12* InRHI)
{
	if(InRHI)
	{ 
		ID3D12Device* Device = InRHI->GetDevice();
		if (Device)
		{
			Delight::ThrowIfFailed(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
				DELIGHT_IID_PPV_ARGS(&commandAllocator)));

			Delight::ThrowIfFailed(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.GetData(),
				nullptr, DELIGHT_IID_PPV_ARGS(&commandList)));
		}
	}
}

void CDX12_CommandList::Reset()
{
	// allocator는 할당된 commandlist가 GPU실행이 끝나야지 리셋가능.
	commandAllocator->Reset();
	// 항상 쓰기 전에 리셋해줘야됨.
	commandList->Reset(commandAllocator.GetData(), nullptr);
}

void CDX12_CommandList::Close()
{
	commandList->Close();
}

void CDX12_CommandList::SetPipeline(Delight::Comptr<ID3D12PipelineState> InPipeline)
{
	commandList->SetPipelineState(InPipeline.GetData());
}

static Delight::CCritialSection GCommandListExecuteCS;

void CDX12_CommandList::Execute(Delight::Comptr<ID3D12CommandQueue> InQueue)
{
	Delight::CScopedCS CS(GCommandListExecuteCS); // thread safe..?
	ID3D12CommandList* CommandLists[] = { commandList.GetData() };

	InQueue->ExecuteCommandLists(1, CommandLists);
}

Delight::Comptr<ID3D12GraphicsCommandList> CDX12_CommandList::Get()
{
	return commandList;
}

