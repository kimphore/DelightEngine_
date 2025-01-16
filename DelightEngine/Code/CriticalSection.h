#pragma once

#include "Define.h"
#include <Windows.h>

BEGIN(Delight)

class CCritialSection
{
public:
	CCritialSection();
	~CCritialSection();

	void Enter();
	void Leave();

private:
	CRITICAL_SECTION CS;
};

class CScopedCS
{
public:
	CScopedCS(CCritialSection& InCS);
	~CScopedCS();

private:
	CCritialSection CS;
};

END