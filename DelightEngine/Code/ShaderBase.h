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
	Tchar fileName[128];
	Char entryPoint[64];
	UInt32 complieFlag;
	EShaderType shaderType;

	FShaderDesc()
		: complieFlag(0),
		shaderType(SHADER_END)
	{
		Delight::memzero(fileName, sizeof(Tchar) * 128);
		Delight::memzero(entryPoint, sizeof(Char) * 64);
	}
};


class IDelightShaderBase
{
public:
	IDelightShaderBase()
		:bInitialized(FALSE)
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
	Bool8 bInitialized;	 
};