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
	class ENGINE_DLL CStackMemoryPool : public IMemoryPool
	{
	public:
		CStackMemoryPool();
		CStackMemoryPool(uint64 _poolSize);
		~CStackMemoryPool();

	public:
		virtual void* allocate(uint64 Size);
		virtual void* reallocate(void* pointer, uint64 Size);
		virtual void reset();
		virtual void deallocate();
		virtual void release();


	private:
		void* allocateStack(uint64 Size);

	private:
		byte* StackCurrentPointer; // current unused stack pointer.(top)
		byte* StackStartPointer; // stack start point.
		uint64 UsedSize;
		uint64 TotalSize;
		bool8 DirtyFlag;
	};

}