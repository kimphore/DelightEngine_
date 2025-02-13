#pragma once

#include "Include.h"

#include "eastl/atomic.h"

// 레퍼런트 오브젝트 기반 클래스

class CDelightReferenceObject
{
public:
	void AddRef() {
		ReferenceCount.fetch_add(1, eastl::memory_order_relaxed);
	}
	bool8 Release() {
		ReferenceCount.fetch_sub(1, eastl::memory_order_relaxed);

		if (GetReferenceCount() == 0)
		{
			delete this;

			return true; // do null
		}

		return false;
	}

	int32 GetReferenceCount() {
		return ReferenceCount.load();
	}

protected:
	eastl::atomic<int32> ReferenceCount = 0;
};