#include "WindowsMemory.h"
#include "Logger.h"

result Delight::CWindowsMemory::Initialize()
{
	Type = EMemoryType::SystemAllocation;

	return 0;
}

void Delight::CWindowsMemory::Shutdown()
{

}

void* Delight::CWindowsMemory::Alloc(uint64 newSize)
{
	return ::malloc(newSize);
}

void Delight::CWindowsMemory::Free(void* allocatedPointer)
{
	::free(allocatedPointer);
}

void* Delight::CWindowsMemory::Realloc(void* allocatedPointer, uint64 newSize)
{
	return ::realloc(allocatedPointer, newSize);
}

uint64 Delight::CWindowsMemory::GetSize(void* allocatedPointer)
{
	return _msize(allocatedPointer);
}
