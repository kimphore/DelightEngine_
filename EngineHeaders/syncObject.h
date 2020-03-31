#pragma once

/*!
 * \file syncObject.h
 * \date 2020/03/31 23:01
 *
 * \author kimphore
 * Contact: user@company.com
 *
 * \brief scoped critical section.
 *
 * TODO: long description
 *
 * \note
*/

namespace Delight
{
	class CScopedLock
	{
	public:
		CScopedLock()
		{
			InitializeCriticalSectionAndSpinCount(&cs, 100);
			EnterCriticalSection(&cs);
		}

		~CScopedLock()
		{
			LeaveCriticalSection(&cs);
		}

	private:
		CRITICAL_SECTION cs;
	};
}