#include "Include.h"
#include "Logger.h"

#define LOGFILENAME_FORMAT "Launch[%04d-%02d-%02d %02d:%02d:%02d].txt"

void CDelightLogger::Initialize()
{
	// �ð���� ���ϻ���.	
	_tchar filename[128] = TEXT("");
	time_t timer;
	tm timeInfo;

	memset(filename, 0, sizeof(_tchar) * )
	

	timer = time(NULL);
	localtime_s(&timeInfo, &timer);

}

void CDelightLogger::ToggleConsole()
{
	// on/off console.
	bUseConsole = !bUseConsole;

	if (bUseConsole)
	{

	}
	else
	{

	}
}
