#pragma once
#include "MemoryInterface.h"

namespace Delight
{
	/*
		Memory Allocation Functions
	*/

	static void* malloc(uint64 size)
	{
		return GMemory->Alloc(size);
	}

	static void free(void* allocatedPointer)
	{
		if (allocatedPointer == nullptr)
			return;

		GMemory->Free(allocatedPointer);
	}

	static void* realloc(void* allocatedPointer, uint64 newSize)
	{
		void* newAllocatedPointer = nullptr;

		if (newSize == 0 || allocatedPointer == nullptr)
			return newAllocatedPointer; // size_t unsigned.

		return GMemory->Realloc(allocatedPointer, newSize);
	}

	static uint64 getAllocatedSize(void* allocatedPointer)
	{
		if (allocatedPointer == nullptr)
			return 0;

		return GMemory->GetSize(allocatedPointer);
	}
}

void* operator new(size_t size)
{
	return GMemory->Alloc(size);
}

void* operator new[](size_t size)
{
	return GMemory->Alloc(size);
}

void operator delete(void* pointer)  throw()
{
	GMemory->Free(pointer);
}

void operator delete[](void* pointer)  throw()
{
	GMemory->Free(pointer);
}
