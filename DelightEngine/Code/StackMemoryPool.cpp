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

void Delight::CStackMemoryPool::deallocate()
{
	//�ǹ̰� ������..?
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