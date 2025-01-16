#include "DX12_CommandList.h"
#include "Exception.h"
#include "CriticalSection.h"

void CDX12_CommandList::Init(Delight::Comptr<ID3D12Device> InDevice)
{
	Delight::ThrowIfFailed(InDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		DELIGHT_IID_PPV_ARGS(&commandAllocator)));

	Delight::ThrowIfFailed(InDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.GetData(),
		nullptr, DELIGHT_IID_PPV_ARGS(&commandList)));
}

void CDX12_CommandList::Reset()
{
	// allocator�� �Ҵ�� commandlist�� GPU������ �������� ���°���.
	commandAllocator->Reset();
	// �׻� ���� ���� ��������ߵ�.
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

