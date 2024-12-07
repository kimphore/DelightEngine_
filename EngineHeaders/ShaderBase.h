#pragma once

#include "comptr.h"

/*!
* \file ShaderBase.h
* \date 2019/08/02 12:53
*
* \author kimphore
* Contact: user@company.com
*
* \brief Shader Base Class.
*
* TODO: long description
*
* \note
*/

struct FShaderDesc
{
	tchar fileName[128];
	char entryPoint[64];
	uint32 complieFlag;
	EShaderType shaderType;

	FShaderDesc()
		: complieFlag(0),
		shaderType(SHADER_END)
	{
		Delight::memzero(fileName, sizeof(tchar) * 128);
		Delight::memzero(entryPoint, sizeof(char) * 64);
	}
};


class IDelightShaderBase
{
public:
	IDelightShaderBase()
		:bInitialized(false)
	{}
public:
	EShaderType GetShaderType() {
		return ShaderType;
	}

	Delight::Comptr<ID3DBlob> GetShaderObject() {
		return ShaderObject;
	}
	
	virtual void Initialize(FShaderDesc* Desc) PURE; // Create Or Cache.

	virtual void CompileShader();
protected:
	Delight::FMD5Hash shaderHash; //
	FShaderDesc shaderDescription;
	EShaderType ShaderType;
	Delight::Comptr<ID3DBlob> ShaderObject;
	Delight::Comptr<ID3DBlob> ShaderErr;
	bool8 bInitialized;	 
};