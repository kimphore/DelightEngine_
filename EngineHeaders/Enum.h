#pragma once

enum ERetType
{
	RET_SUCCES,
	RET_FAIL,

};

enum ESystemSettingEnum
{
	CORE_RHI_D3D12,
	CORE_RHI_END = 4,

};

enum ELogType
{
	LOG_NORMAL,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_END
};