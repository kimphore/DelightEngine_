#pragma once

#include <thread>

#include "Typedef.h"

// extern blah blah.

extern uint64 GGameThreadFrame;
extern uint64 GRenderThreadFrame;

extern std::thread::id GGameThreadID;
extern std::thread::id GRenderThreadID;