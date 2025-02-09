#pragma once
#include "MemoryInterface.h"

namespace Delight
{
	/*
		Memory Allocation Functions
	*/

	static uint32 GMemoryType = EMemoryType::SystemAllocation;

	static void* malloc(uint64 size)
	{
		Delight::InitializeMemory(GMemoryType);

		return GMemory->Alloc(size);
	}

	static void free(void* allocatedPointer)
	{
		if (allocatedPointer == nullptr)
			return;

		Delight::InitializeMemory(GMemoryType);

		GMemory->Free(allocatedPointer);
	}

	static void* realloc(void* allocatedPointer, uint64 newSize)
	{
		void* newAllocatedPointer = nullptr;

		if (newSize == 0 || allocatedPointer == nullptr)
			return newAllocatedPointer; // size_t unsigned.

		Delight::InitializeMemory(GMemoryType);

		return GMemory->Realloc(allocatedPointer, newSize);
	}

	static uint64 getAllocatedSize(void* allocatedPointer)
	{
		if (allocatedPointer == nullptr)
			return 0;

		Delight::InitializeMemory(GMemoryType);

		return GMemory->GetSize(allocatedPointer);
	}
}

void* operator new(size_t size)
{
	Delight::InitializeMemory(Delight::GMemoryType);
	return GMemory->Alloc(size);
}

void* operator new[](size_t size)
{
	Delight::InitializeMemory(Delight::GMemoryType);
	return GMemory->Alloc(size);
}

void operator delete(void* pointer)  throw()
{
	Delight::InitializeMemory(Delight::GMemoryType);
	GMemory->Free(pointer);
}

void operator delete[](void* pointer)  throw()
{
	Delight::InitializeMemory(Delight::GMemoryType);
	GMemory->Free(pointer);
}
