#include "MemoryInterface.h"
#include "TBBMemory.h"
#include "WindowsMemory.h"

Delight::IMemoryBase* GMemory = nullptr;

EEngineResult Delight::InitializeMemory(uint32 Type)
{
	if (GMemory != nullptr)
	{
		return EEngineResult::SUCESS;
	}
	Delight::ASSERT(GMemory == nullptr, "Memory Class Already Initialized.");

	// placement new
	switch (Type)
	{
	case EMemoryType::SystemAllocation:
		GMemory = (CWindowsMemory*)malloc(sizeof(CWindowsMemory));
		GMemory = new(GMemory) CWindowsMemory;
		break;
	case EMemoryType::TBBAllocation:
		GMemory = (CTBBMemory*)malloc(sizeof(CTBBMemory));
		GMemory = new(GMemory) CTBBMemory;
		break;
	}
	
	Delight::ASSERT((GMemory && (GMemory->Initialize() == 0)) || GMemory == nullptr, "Memory Class Initialize Failed.");

	return EEngineResult::SUCESS;
}

void Delight::ShutdownMemory()
{
	GMemory->Shutdown();
	free(GMemory);
	GMemory = nullptr;
}
