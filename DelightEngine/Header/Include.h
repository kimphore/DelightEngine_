#pragma once

#include "DefineFlag.h" // 기능 컨트롤을 위해 가장 먼저 선언.

//#include <vld.h> // for memory leak detect.
//

#if USE_EASTL_DLL
// @begin for EASTL new override.
inline void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	return new unsigned char[size];
}

inline void* __cdecl operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	return new unsigned char[size];
}

#pragma comment(lib, "eastl.lib") // 64bit debug version lib.
// @end -prkim
//#include "eastl/string.h"
#endif

// tbb
#include "tbb/include/tbb/tbbmalloc_proxy.h"
#include "tbb/include/tbb/scalable_allocator.h"

//#include <d3d11.h>
//#include <d3dx11.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
//#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <assert.h>
#include <windows.h>
//#include <dinput.h>
#include <wrl.h>
#include <time.h>
#include <memory.h>

#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <string.h>
#include <chrono>
#include <random>
#include <ctime>
#include <functional>
#include <utility>
#include <memory>
#include <fstream>
#include <ostream>
#include <stdio.h>
#include <functional>
#include <d3dcompiler.h>

// using namespace..?

using namespace Microsoft::WRL;

// internal utility headers.
#include "syncObject.h"

// internal include headers.
#include "Typedef.h"
#include "Exception.h"
#include "Define.h"
#include "Enum.h"
#include "struct.h"
#include "Function.h"

#pragma comment(lib, "D3D12.lib")

// ignore warning.
#pragma warning(disable : 4251) // ignore std dll export warning..
#pragma warning(disable : 4996) // ignore using unsafe functions..

#include "logger.h"
#include "comptr.h"