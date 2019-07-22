#pragma once

class CDelightLogger
{
public:
	CDelightLogger();
	~CDelightLogger();

public:
	void Initialize();
	void ToggleConsole();
private:
	wofstream logFile;
	_bool bUseConsole;
};