#pragma once
#include "Include.h"

/*
* upload buffer는 여기서.. 크게 할당해놓고 잘라쓰기..?
*/
struct FChunkInfo
{
	uint32 PoolIndex;
	uint64 Offset;
	uint64 Size;
};

class CDX12_BufferInterface;
class CDX12_BufferPool
{

};