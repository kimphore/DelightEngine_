#pragma once

#include "Define.h"

BEGIN(Delight)

// max queue size 128MB?
#define MAX_QUEUE_SIZE (128 * 1024 * 1024)

class CRenderCommandInterface
{
	virtual void DoCommand() = 0;
	virtual uint32 GetCommandSize() = 0;
};

class CRenderCommandQueue
{
public:
	void Initialize();

public:
	CRenderCommandQueue* Dequeue();
	void Enqueue(CRenderCommandInterface* InCommnad);
private:
	byte* Buffer = nullptr;
	uint64 CurrentPos = 0;
};

extern CRenderCommandQueue GRenderCommandQueue;

#define ENQUEUE_RENDER_COMMAND(CommandClassName, Lambda) \
	class CommandClassName : public CRenderCommandQueue \
	{\
		virtual void DoCommand() Lambda;\
		virtual uint32 GetCommandSize() {return sizeof(CommandClassName);}\
	};

END