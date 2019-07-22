#pragma once

class CDelightLogger
{
public:
	void Initialize();
	void ToggleConsole();
private:
	FILE* LoggerFile;
	_bool bUseConsole;
};