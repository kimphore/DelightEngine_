#pragma once

#include "Include.h"
#include "ShaderBase.h"

class CVertexShader : public IDelightShaderBase
{
public:
	CVertexShader()
	{}

public:
	virtual void Initialize(FShaderDesc* Desc);
	virtual void Compile();
};