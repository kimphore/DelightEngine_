#pragma once

#include "Typedef.h"
#include <thread>

class CDelightRenderThread
{
public:
	hres InitializeAndStartThread();
	void DoWork();
	void Join();

private:
	std::thread Thread;
};