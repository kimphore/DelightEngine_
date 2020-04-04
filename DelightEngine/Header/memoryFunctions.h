#pragma once

namespace Delight
{
	/*
	Memory Allocation Functions
*/

	static inline Bool8 isAllocatedFormTBB(void* allocatedPointer)
	{
		if (allocatedPointer == nullptr)
			return false;

		return scalable_msize(allocatedPointer) != 0;
	}

	static void* malloc(size_t size)
	{
		void* allocatedPointer = nullptr;
#if USE_TBB_ALLOCATION
		allocatedPointer = scalable_malloc(size);
#else
		allocatedPointer = ::malloc(size);
#endif

		return allocatedPointer;
	}

	static void free(void* allocatedPointer)
	{
		if (allocatedPointer == nullptr)
			return;

#if USE_TBB_ALLOCATION
		// if allocated from tbb library
		if (isAllocatedFormTBB(allocatedPointer))
		{
			scalable_free(allocatedPointer);
		}
#else
		free(allocatedPointer);
#endif
	}

	static void* realloc(void* allocatedPointer, size_t newSize)
	{
		void* newAllocatedPointer = nullptr;

		if (newSize == 0 || allocatedPointer == nullptr)
			return newAllocatedPointer; // size_t unsigned.

#if USE_TBB_ALLOCATION
		if (isAllocatedFormTBB(allocatedPointer))
		{
			newAllocatedPointer = scalable_realloc(allocatedPointer, newSize);
		}
#else
		newAllocatedPointer = ::realloc(allocatedPointer, newSize);
#endif

		return newAllocatedPointer;
	}

	static size_t getAllocatedSize(void* allocatedPointer)
	{
		if (allocatedPointer == nullptr)
			return 0;

		// tbb, malloc 구분하지 않고 범용적으로 사용하기 위해.
#if USE_TBB_ALLOCATION
		size_t scalableSize = scalable_msize(allocatedPointer);
		size_t mallocSize = _msize(allocatedPointer);
#else
		size_t scalableSize = 0;
		size_t mallocSize = _msize(allocatedPointer);
#endif

		return max(scalableSize, mallocSize);
	}
}