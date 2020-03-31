#include "Include.h"
#include "PixelShader.h"

void CPixelShader::Initialize(FShaderDesc* Desc)
{
	// for shader debugging flag.
	UInt32 CompileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	// md5�� ���߿����ְ� ������ �ܼ��ϰ�..
	D3DCompileFromFile(Desc->fileName, nullptr, nullptr,
		Desc->entryPoint, "ps_5_0", CompileFlag,
		0, &ShaderObject, &ShaderErr);
}

void CPixelShader::Compile()
{

}
