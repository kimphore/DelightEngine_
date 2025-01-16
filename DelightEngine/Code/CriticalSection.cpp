#include "CriticalSection.h"


Delight::CCritialSection::CCritialSection()
{
	InitializeCriticalSection(&CS);
}

Delight::CCritialSection::~CCritialSection()
{
	DeleteCriticalSection(&CS);
}

void Delight::CCritialSection::Enter()
{
	EnterCriticalSection(&CS);
}

void Delight::CCritialSection::Leave()
{
	LeaveCriticalSection(&CS);
}

Delight::CScopedCS::CScopedCS(Delight::CCritialSection& InCS)
	:CS(InCS)
{
	CS.Enter();
}

Delight::CScopedCS::~CScopedCS()
{
	CS.Leave();
}
