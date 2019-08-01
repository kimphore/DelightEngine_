#include "Include.h"
#include "Logger.h"
#include <tchar.h>
#include <stdio.h>
#include <iostream>

#define LOGFILENAME_FORMAT "..\\Log\\Launch[%04d-%02d-%02d %02d.%02d.%02d].txt"
#define TIME_FORMAT "[%04d-%02d-%02d %02d.%02d.%02d]"

CDelightLogger GLogger;

const _int logColorIndex[5] = {
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

void CDelightLogger::Initialize()
{
	std::locale::global(std::locale("kor"));

	// 시간기반 파일생성.	
	_tchar filename[256];
	time_t timer;
	tm timeInfo;

	memset(filename, 0, sizeof(_tchar) * 256);

	timer = time(NULL);
	localtime_s(&timeInfo, &timer);

	// second parameter is not array size. just array count.
	_stprintf_s(filename, _countof(filename), TEXT(LOGFILENAME_FORMAT), timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
	
	logFile.open(filename);

	GLogger.LogW(LOG_INFO, SYSTEM_LOG_LEVEL, TEXT("Logger Initialized."));
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

		std::ios::sync_with_stdio();
	}
	else
	{
		FreeConsole();
		// close console.
	}
}

void CDelightLogger::LogW(ELogType logType, _int LogLevel, _tchar* format, ...)
{
	_tchar StringBuffer[512] = TEXT("");
	_tchar header[64] = TEXT("");
	_tchar timeHeader[64] = TEXT("");
	time_t timer;
	tm timeInfo;
	_int logColor = _int(logType);

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
