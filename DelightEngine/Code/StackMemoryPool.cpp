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
 * \brief Stack������ MemoryPool.
		  ���������� �ʰ�, �ֹ߼��� �����͵��� ���� ���(ex] Frame���� �ʱ�ȭ)
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

Delight::CStackMemoryPool::CStackMemoryPool(size_t _poolSize)
{
	allocate(_poolSize);
}

// return allocated or reallocated memory pointer(top)
void* Delight::CStackMemoryPool::allocate(size_t Size)
{
	void* retPointer = StackCurrentPointer;

	updateStackState(Size);
	
	UsedSize += Size;
	StackCurrentPointer += Size;	

	return retPointer;
}

// ��� StackPool�� �������� �ʱ�ȭ�ǹǷ�.. 
// �� �Ҵ��� ���ϰ� �׳� allocation�̶� ����.
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

// real memory allocation.
void Delight::CStackMemoryPool::allocateStack(size_t Size)
{
	if (StackStartPointer == nullptr)
	{
		// first allocation.
		StackStartPointer = (Byte*)Delight::malloc(Size);
		reset();
	}
	else
	{
		// pool reallocation.
		StackStartPointer = (Byte*)Delight::realloc(StackStartPointer, Size);
	}

	TotalSize = Size;
	
	Delight::ASSERT(
		Delight::getAllocatedSize(StackStartPointer) != Size,
		"Stack Allocation Failed!"
	);
}

// check that need reallocation or allocation.
void Delight::CStackMemoryPool::updateStackState(size_t Size)
{
	size_t afterSize = UsedSize + Size;

	if (TotalSize < afterSize)
	{
		// alloc or realloc.
		allocateStack(Size);
	}
}