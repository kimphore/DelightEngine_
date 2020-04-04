#include "Include.h"
#include "allocator.h"
#include "MemoryPoolInterface.h"
#include "StackMemoryPool.h"

/*!
 * \file allocator.cpp
 * \date 2020/03/29 13:27
 *
 * \author kimphore
 * Contact: user@company.com
 *
 * \brief allocator에서 생성 요청을 받으면 memorypool에서 실제로 할당받아서 넘겨주도록.
 *
 * TODO: long description
 *
 * \note
*/
Delight::CAllocator::CAllocator(const char* pName /*= "default_allocator"*/)
{

}

void* Delight::CAllocator::allocate(size_t n, int flags /*= 0*/)
{
	if (MemoryPool == nullptr)
		return nullptr;

	return MemoryPool->allocate(n);
}

void Delight::CAllocator::deallocate(void* p, size_t n)
{
	if (MemoryPool == nullptr)
		return;
}

void* Delight::CAllocator::allocate(size_t n, size_t alignment, size_t offset, int flags /*= 0*/)
{
	if (MemoryPool == nullptr)
		return nullptr;

	return nullptr;
}