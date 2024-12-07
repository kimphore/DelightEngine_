#pragma once
#include "MemoryInterface.h"

namespace Delight
{
	class ENGINE_DLL CWindowsMemory : public IMemoryBase
	{
	public:
		virtual result Initialize() override;
		virtual void Shutdown() override;

	public:
		void* Alloc(uint64 newSize) override;
		void Free(void* allocatedPointer) override;
		void* Realloc(void* allocatedPointer, uint64 newSize) override;
		uint64 GetSize(void* allocatedPointer) override;
	};
}