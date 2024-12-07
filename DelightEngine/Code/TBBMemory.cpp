#include "TBBMemory.h"
#include "Logger.h"

result Delight::CTBBMemory::Initialize()
{
	Type = EMemoryType::TBBAllocation;

	return 0;
}

void Delight::CTBBMemory::Shutdown()
{

}

void* Delight::CTBBMemory::Alloc(uint64 newSize)
{
	Delight::ASSERT(newSize > 0, "Wrong Size Allocation.");

	return scalable_malloc(newSize);
}

void Delight::CTBBMemory::Free(void* allocatedPointer)
{
	scalable_free(allocatedPointer);
}

void* Delight::CTBBMemory::Realloc(void* allocatedPointer, uint64 newSize)
{
	void* NewPointer = nullptr;

	if (allocatedPointer && newSize > 0)
	{
		NewPointer = scalable_realloc(allocatedPointer, newSize);
	}

	return NewPointer;
}

uint64 Delight::CTBBMemory::GetSize(void* allocatedPointer)
{
	return scalable_msize(allocatedPointer);
}
