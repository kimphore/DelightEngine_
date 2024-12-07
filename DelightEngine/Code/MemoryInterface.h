#pragma once
#include "Include.h"

namespace Delight
{
	// must call first.
	ENGINE_DLL EEngineResult InitializeMemory(uint32 Type);
	ENGINE_DLL void ShutdownMemory();

	class ENGINE_DLL IMemoryBase
	{
	public:
		EMemoryType GetMemoryType() { return Type; }
		virtual result Initialize() PURE;
		virtual void Shutdown() PURE;
		
	public:
		virtual void* Alloc(uint64 newSize) PURE;
		virtual void Free(void* allocatedPointer) PURE;
		virtual void* Realloc(void* allocatedPointer, uint64 newSize) PURE;
		virtual uint64 GetSize(void* allocatedPointer) PURE;

	protected:
		EMemoryType Type;
	};
}

extern class Delight::IMemoryBase* GMemory;