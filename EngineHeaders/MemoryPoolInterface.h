#pragma once
#include "Include.h"

/*!
 * \file MemoryPoolInterface.h
 * \date 2020/02/01 20:02
 *
 * \author kimphore
 * Contact: user@company.com
 *
 * \brief MemoryPool Interface. Simple Ver..
 *
 * TODO: long description
 *
 * \note
*/

namespace Delight
{
	// replace define.
	enum EMemoryPoolConstant
	{
		EMPC_DefaultPoolSize = 1024 * 1024, // 1MB.
	};

	class IMemoryPool
	{
	public:
		IMemoryPool()
		{}

		virtual ~IMemoryPool()
		{}

	public:
		virtual void* allocate(size_t Size) PURE_FUNCTION;
		virtual void* reallocate(void* pointer, size_t Size) PURE_FUNCTION;
		virtual void reset() PURE_FUNCTION;
	};
}