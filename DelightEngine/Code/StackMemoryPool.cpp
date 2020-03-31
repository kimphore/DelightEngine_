#include "StackMemoryPool.h"

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
	: CurrentStackPointer(nullptr),
	StackBottomPointer(nullptr),
	UsedSize(0),
	TotalSize(0),
	DirtyFlag(false)
{

}

Delight::CStackMemoryPool::CStackMemoryPool(size_t _poolSize)
{

}

void* Delight::CStackMemoryPool::allocate(UInt64 Size)
{
	
	return nullptr;
}

void* Delight::CStackMemoryPool::reallocate(void* pointer, UInt64 Size)
{
	return nullptr;
}

void Delight::CStackMemoryPool::reset()
{

}

void Delight::CStackMemoryPool::allocateStack(size_t)
{

}
