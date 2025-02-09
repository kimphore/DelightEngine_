#pragma once

#define DIRECTINPUT_VERSION 0x0800 // For DirectInput


#define USING(_namespace) using namespace _namespace;

#define BEGIN(_namespace) namespace _namespace {
#define END }

#ifdef ENGINE_EXPORT
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define SAFE_DELETE(Target)	\
	{						\
		delete Target;		\
		Target = nullptr;	\
	}

#define SAFE_RELEASE(Target)	\
	{							\
		Target->Release();		\
		Target = nullptr;		\
	}

#define _count(array) (unsigned int)(sizeof(array) / sizeof(array[0]))

#define FOR_RANGE(index, begin, end) for(Int32 index = begin; index < end; ++index)

#define SYSTEM_LOG_LEVEL 1000

#define PURE_FUNCTION = 0

#define INDEX_NONE -1

#define D3D_DEBUG 1

#define DEBUG_D3D_RESOURCE_STRING 1 && D3D_DEBUG