#pragma once

class ENGINE_DLL CDelightLogger
{
public:
	CDelightLogger();
	~CDelightLogger();

public:
	void Initialize();
	void ToggleConsole();

public:
	void LogW(ELogType logType, _int LogLevel,  _tchar* format, ...);
	void SetIgnoreLogLevel(_int logLevel) {
		ignoreLogLevel = logLevel;
	};
private:
	std::wofstream logFile;
	_int  ignoreLogLevel;
	_bool bUseConsole;
};

extern ENGINE_DLL CDelightLogger GLogger;