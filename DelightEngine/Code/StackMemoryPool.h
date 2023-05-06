#pragma once

#include "MemoryPoolInterface.h"

/*!
 * \file StackMemoryPool.h
 * \date 2020/03/29 13:33
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

namespace Delight
{
	class ENGINE_DLL CStackMemoryPool : public IMemoryPool
	{
	public:
		CStackMemoryPool();
		CStackMemoryPool(size_t _poolSize);
		~CStackMemoryPool();

	public:
		virtual void* allocate(size_t Size);
		virtual void* reallocate(void* pointer, size_t Size);
		virtual void reset();
		virtual void deallocate();
		virtual void release();


	private:
		void* allocateStack(size_t Size);

	private:
		Byte* StackCurrentPointer; // current unused stack pointer.(top)
		Byte* StackStartPointer; // stack start point.
		size_t UsedSize;
		size_t TotalSize;
		Bool8 DirtyFlag;
	};

}