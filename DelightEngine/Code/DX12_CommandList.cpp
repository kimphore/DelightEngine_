#include "DX12_CommandList.h"
#include "Include.h"
#include "Exception.h"
#include "CriticalSection.h"
#include "RHI_DX12Device.h"
#include "DX12_CommandListPool.h"

CDX12_CommandList::CDX12_CommandList(const CDX12_CommandList& InList)
	: Device(InList.Device),
	commandAllocator(InList.commandAllocator),
	commandList(InList.commandList),
	CommandListPool(InList.CommandListPool),
	bClosed(InList.bClosed)
{
	bInitialized = true;
}

CDX12_CommandList::~CDX12_CommandList()
{
	if (CommandListPool && IsValid())
	{
		CommandListPool->ReturnToPool(this);
	}
}

void CDX12_CommandList::Initialize(Delight::Comptr<ID3D12Device> InDevice, CDX12_CommandListPool* InListPool)
{
	Device = InDevice;
	if (Device.IsValid())
	{
		Delight::ThrowIfFailed(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			DELIGHT_IID_PPV_ARGS(&commandAllocator)));

		Delight::ThrowIfFailed(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.GetData(),
			nullptr, DELIGHT_IID_PPV_ARGS(&commandList)));
	}

	CommandListPool = InListPool;
}

void CDX12_CommandList::Reset()
{
	// 이건 여러번 호출해도 상관이없음...(d3ddebug에서는 에러..)
	Close();

	// allocator는 할당된 commandlist가 GPU실행이 끝나야지 리셋가능.
	commandAllocator->Reset();
	
	// 항상 쓰기 전에 리셋해줘야됨.
	commandList->Reset(commandAllocator.GetData(), nullptr);

	bClosed = false;
}

void CDX12_CommandList::Close()
{
	if (bClosed == false)
	{
		commandList->Close();
		bClosed = true;
	}
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

CDX12_CommandList& CDX12_CommandList::operator=(CDX12_CommandList other)
{
	if (&other == this)
	{
		return *this;
	}

	std::swap(this->Device, other.Device);
	std::swap(this->commandAllocator, other.commandAllocator);
	std::swap(this->commandList, other.commandList);
	std::swap(this->CommandListPool, other.CommandListPool);
	std::swap(this->bInitialized, other.bInitialized);
	std::swap(this->bClosed, other.bClosed);

	return *this;

}

Delight::Comptr<ID3D12GraphicsCommandList> CDX12_CommandList::Get()
{
	return commandList;
}

