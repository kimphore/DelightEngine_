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

enum EShaderType
{
	SHADER_VERTEX,
	SHADER_PIXEL,
	// geometry, tessl, ray, etc..
	SHADER_END

};
enum EMemoryType
{
	SystemAllocation,
	TBBAllocation,
};

enum EEngineResult
{
	SUCESS,
	FAIL,

};