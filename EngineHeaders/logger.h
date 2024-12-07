#pragma once

class ENGINE_DLL CDelightLogger
{
public:
	CDelightLogger();
	~CDelightLogger();

public:
	EEngineResult Initialize();
	void ToggleConsole();

public:
	void LogW(ELogType logType, int32 LogLevel,  tchar* format, ...);
	void Log(ELogType logType, int32 LogLevel, char* format, ...);
	void SetIgnoreLogLevel(int32 logLevel) {
		ignoreLogLevel = logLevel;
	};
private:
	std::wofstream logFile;
	int32  ignoreLogLevel;
	bool8 bUseConsole;
};

extern ENGINE_DLL CDelightLogger* GLogger;

namespace Delight
{
	void InitializeLogger();
	void ShutdownLogger();
}


#define logf(format, ...) GLogger->LogW(LOG_NORMAL, 100, format, __VA_ARGS__)
#define warnf(format, ...) GLogger->LogW(LOG_WARN, 500, format, __VA_ARGS__)
#define errf(format, ...) GLogger->LogW(LOG_ERROR, 1000, format, __VA_ARGS__)
#define infof(format, ...) GLogger->LogW(LOG_INFO, 100, format, __VA_ARGS__)

#define log(format, ...) GLogger->Log(LOG_NORMAL, 100, format, __VA_ARGS__)
#define warn(format, ...) GLogger->Log(LOG_WARN, 500, format, __VA_ARGS__)
#define err(format, ...) GLogger->Log(LOG_ERROR, 1000, format, __VA_ARGS__)
#define info(format, ...) GLogger->Log(LOG_INFO, 100, format, __VA_ARGS__)
