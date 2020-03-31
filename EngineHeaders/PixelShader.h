#pragma once

#include "ShaderBase.h"

class CPixelShader : public IDelightShaderBase
{
public:
	CPixelShader()
	{}

public:
	virtual void Initialize(FShaderDesc* Desc);
	virtual void Compile();
};