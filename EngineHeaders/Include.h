#pragma once

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

// using namespace..?

using namespace Microsoft::WRL;

// internal include headers.

#include "Exception.h"
#include "Typedef.h"
#include "Define.h"
#include "Enum.h"

#pragma comment(lib, "D3D12.lib")

// ignore warning.
#pragma warning(disable : 4251) // ignore std dll export warning..
#pragma warning(disable : 4996) // ignore using unsafe functions..