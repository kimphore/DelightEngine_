#pragma once

/*!
 * \file DefineFlag.h
 * \date 2020/03/29 16:14
 *
 * \author kimphore
 * Contact: user@company.com
 *
 * \brief Define을 통해 C++ 기능 On/Off 제어
		  형식은 무조건 #define name 0or1. 0 == off, 1 == on;
 *
 * TODO: long description
 *
 * \note
*/

#define USE_EASTL_DLL 1
#define USE_TBB_ALLOCATION 1
#define MEMORY_ALLOCATION_TYPE USE_TBB_ALLOCATION // See EMemoryType 0 : Windows Allocation, 1 : TBB Allocation