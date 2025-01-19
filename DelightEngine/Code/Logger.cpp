#include "Include.h"
#include "Logger.h"
#include <tchar.h>
#include <stdio.h>
#include <iostream>

#define LOGFILENAME_FORMAT "..\\Log\\Launch[%04d-%02d-%02d %02d.%02d.%02d].txt"
#define TIME_FORMAT "[%04d-%02d-%02d %02d.%02d.%02d]"

extern CDelightLogger* GLogger = nullptr;


void Delight::InitializeLogger()
{
	GLogger = new CDelightLogger;
}

void Delight::ShutdownLogger()
{
	delete GLogger;
	GLogger = nullptr;
}

const int32 logColorIndex[5] = {
	15, // LOG_NORMAL
	7,  // LOG_INFO
	14, // LOG_WARN
	12, // LOG_ERROR
};

CDelightLogger::CDelightLogger()
	: ignoreLogLevel(-1),
	  bUseConsole(false)
{
	Initialize();
}

CDelightLogger::~CDelightLogger()
{
	logFile.close();
	
	if (bUseConsole)
	{
		FreeConsole();
	}
}

EEngineResult CDelightLogger::Initialize()
{
	std::locale::global(std::locale("kor"));

	// 시간기반 파일생성.	
	tchar filename[256];
	time_t timer;
	tm timeInfo;

	memset(filename, 0, sizeof(tchar) * 256);

	timer = time(NULL);
	localtime_s(&timeInfo, &timer);

	// second parameter is not array size. just array count.
	_stprintf_s(filename, _countof(filename), TEXT(LOGFILENAME_FORMAT), timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
	
	logFile.open(filename);

	return EEngineResult::SUCESS;
}

void CDelightLogger::ToggleConsole()
{
	// on/off console.
	bUseConsole = !bUseConsole;

	if (bUseConsole)
	{
		// open console.
		AllocConsole();
		SetConsoleTitle(TEXT("Console"));

		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
		freopen("CONERR$", "w", stderr);

		setlocale(LC_ALL, "");

		std::ios::sync_with_stdio(true);
	}
	else
	{
		FreeConsole();
		// close console.
	}
}

void CDelightLogger::LogW(ELogType logType, int32 LogLevel, tchar* format, ...)
{
	tchar StringBuffer[512] = TEXT("");
	tchar header[64] = TEXT("");
	tchar timeHeader[64] = TEXT("");
	time_t timer;
	tm timeInfo;
	int32 logColor = int32(logType);

	timer = time(NULL);
	localtime_s(&timeInfo, &timer);

	switch (logType)
	{
	case LOG_INFO:
		_stprintf_s(header, _countof(header), TEXT("[INFO]"));
		break;
	case LOG_WARN:
		_stprintf_s(header, _countof(header), TEXT("[WARN]"));
		break;
	case LOG_ERROR:
		_stprintf_s(header, _countof(header), TEXT("[ERR]"));
		break;
	}

	_stprintf_s(timeHeader, _countof(timeHeader), TEXT(TIME_FORMAT), timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
		timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);

	va_list Marker;

	va_start(Marker, format);
	vswprintf(StringBuffer, format, Marker);
	va_end(Marker);

	if (bUseConsole && ignoreLogLevel < LogLevel)
	{	
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), logColorIndex[logColor]);
		std::wcout << timeHeader << header << StringBuffer << std::endl;
	}

	logFile << timeHeader;
	logFile << header;
	logFile << StringBuffer<< std::endl;
}
// for char based log
void CDelightLogger::Log(ELogType logType, int32 LogLevel, char* format, ...)
{
	char StringBuffer[512] = "";
	char header[64] = "";
	char timeHeader[64] = "";
	time_t timer;
	tm timeInfo;
	int32 logColor = int32(logType);

	timer = time(NULL);
	localtime_s(&timeInfo, &timer);

	switch (logType)
	{
	case LOG_INFO:
		sprintf_s(header, sizeof(char) * 64, "[INFO]");
		break;
	case LOG_WARN:
		sprintf_s(header, sizeof(char) * 64, "[WARN]");
		break;
	case LOG_ERROR:
		sprintf_s(header, sizeof(char) * 64, "[ERR]");
		break;
	}

	sprintf_s(timeHeader, sizeof(char) * 64, TIME_FORMAT, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
		timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);

	va_list Marker;

	va_start(Marker, format);
	vsprintf(StringBuffer, format, Marker);
	va_end(Marker);

	if (bUseConsole && ignoreLogLevel < LogLevel)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), logColorIndex[logColor]);
		std::cout << timeHeader << header << StringBuffer << std::endl;
	}

	logFile << timeHeader;
	logFile << header;
	logFile << StringBuffer << std::endl;
}
