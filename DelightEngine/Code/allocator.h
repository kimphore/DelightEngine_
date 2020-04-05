#pragma once

/*!
 * \file allocatorInterface.h
 * \date 2020/02/01 19:38
 *
 * \author kimphore
 * Contact: user@company.com
 *
 * \brief [allocator] -> [new] -> [memory pool] -> [new]!
		  stl과 eastl 둘다 사용 할 수 있도록 interface 맞춰서 제작.
		  
		  20200331 * 할까 했지만.. 기존 stl은 allocator 한계가 명확해 무시하도록..
 *
 * TODO: long description
 *
 * \note
*/

namespace Delight
{
	class IMemoryPool;
	class ENGINE_DLL CAllocator
	{
	public:
		explicit CAllocator(const char* pName = "default_allocator");
		CAllocator(const CAllocator& x);
		CAllocator(const CAllocator& x, const char* pName);
		virtual ~CAllocator();

		void* allocate(size_t n, int flags = 0);
		void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0);
		void  deallocate(void* p, size_t n);
		void  reset() {
			// do not work in here.
			// it will be work in frame allocator or etc.
		}
		void  bind_memory_pool(IMemoryPool* _memoryPool)
		{
			if (MemoryPool)
			{
				// already binded memory pool.
				return;
			}

			if (_memoryPool)
				MemoryPool = _memoryPool;
		}

		const char* get_name() const
		{
			return AllocatorName;
		}
		void set_name(const char* pName)
		{
			AllocatorName = pName;
		}

	protected:
		const char* AllocatorName;
		IMemoryPool* MemoryPool;
	};
}