#pragma once

#include "MemoryPoolInterface.h"

/*!
 * \file StackMemoryPool.h
 * \date 2020/03/29 13:33
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

namespace Delight
{
	class CStackMemoryPool : public IMemoryPool
	{
	public:
		CStackMemoryPool();
		CStackMemoryPool(size_t _poolSize);

	public:
		virtual void* allocate(size_t Size);
		virtual void* reallocate(void* pointer, size_t Size);
		virtual void reset();

	private:
		void allocateStack(size_t Size);
		void updateStackState(size_t Size);

	private:
		Byte* StackCurrentPointer; // current unused stack pointer.(top)
		Byte* StackStartPointer; // stack start point.
		Int32 UsedSize;
		Int32 TotalSize;
		Bool8 DirtyFlag;
	};

}