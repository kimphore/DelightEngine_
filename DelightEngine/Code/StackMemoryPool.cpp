#include "Include.h"
#include "StackMemoryPool.h"
#include "memoryFunctions.h"
#include "logger.h"

/*!
 * \file StackMemoryPool.cpp
 * \date 2020/03/29 13:43
 *
 * \author kimphore
 * Contact: user@company.com
 *
 * \brief Stack형식의 MemoryPool.
		  영속적이지 않고, 휘발성인 데이터들을 위해 사용(ex] Frame마다 초기화)
 *
 * TODO: long description
 *
 * \note
*/

Delight::CStackMemoryPool::CStackMemoryPool()
	: StackCurrentPointer(nullptr),
	StackStartPointer(nullptr),
	UsedSize(0),
	TotalSize(0),
	DirtyFlag(false)
{

}

Delight::CStackMemoryPool::~CStackMemoryPool()
{
	release();
}

Delight::CStackMemoryPool::CStackMemoryPool(size_t _poolSize)
	: StackCurrentPointer(nullptr),
	StackStartPointer(nullptr),
	UsedSize(0),
	TotalSize(0),
	DirtyFlag(false)
{
	allocate(_poolSize);
}

// return allocated or reallocated memory pointer(top)
void* Delight::CStackMemoryPool::allocate(size_t Size)
{
	return allocateStack(Size);
}

// 사실 StackPool은 매프레임 초기화되므로.. 
// 재 할당은 안하고 그냥 allocation이랑 같다.
void* Delight::CStackMemoryPool::reallocate(void* pointer, size_t Size)
{
	size_t currentSize = Delight::getAllocatedSize(pointer);
	void* newAllocation = nullptr;

	if (currentSize > 0)
	{
		// reallocation and copy data.
		newAllocation = allocate(Size);

		if (newAllocation != nullptr)
		{
			memcpy(newAllocation, pointer, currentSize);
		}
	}

	return newAllocation;
}

void Delight::CStackMemoryPool::reset()
{
	if (DirtyFlag)
	{
		StackCurrentPointer = StackStartPointer;
		UsedSize = 0;
		DirtyFlag = false;
	}
}

void Delight::CStackMemoryPool::deallocate()
{
	//의미가 있을까..?
}

void Delight::CStackMemoryPool::release()
{
	if (StackStartPointer)
	{
		Delight::free(StackStartPointer);
		StackStartPointer = nullptr;
		StackCurrentPointer = nullptr;
		UsedSize = 0;
		TotalSize = 0;
		DirtyFlag = false;
	}
}

// real memory allocation.
void* Delight::CStackMemoryPool::allocateStack(uint64 Size)
{
	if (StackStartPointer == nullptr)
	{
		// first allocation.
		StackStartPointer = (byte*)Delight::malloc(Size);
		StackCurrentPointer = StackStartPointer;
		reset();
	}
	else
	{
		// pool reallocation.
		StackStartPointer = (byte*)Delight::realloc(StackStartPointer, Size);
	}

	Delight::ASSERT(StackStartPointer != nullptr, "Stack Allocation Failed!");
	TotalSize = Size;

	return StackStartPointer;
}