#include "Include.h"
#include "Logger.h"

#define LOGFILENAME_FORMAT "..\\Log\\Launch[%04d-%02d-%02d %02d:%02d:%02d].txt"

CDelightLogger::CDelightLogger()
{
	Initialize();
}

CDelightLogger::~CDelightLogger()
{
	logFile.close();
}

void CDelightLogger::Initialize()
{
	std::locale::global(std::locale("kor"));

	// 시간기반 파일생성.	
	_tchar filename[128] = TEXT("");
	time_t timer;
	tm timeInfo;

	timer = time(NULL);
	localtime_s(&timeInfo, &timer);

	_stprintf_s(filename, sizeof(_tchar) * 128, _T(LOGFILENAME_FORMAT), timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
	
	logFile.open(filename);
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
