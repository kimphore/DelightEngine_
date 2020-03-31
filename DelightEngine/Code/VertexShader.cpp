#include "Include.h"
#include "VertexShader.h"

void CVertexShader::Initialize(FShaderDesc* Desc)
{
	if (Desc == nullptr) return;

	// for shader debugging flag.
	UInt32 CompileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	// md5�� ���߿����ְ� ������ �ܼ��ϰ�..
	D3DCompileFromFile(Desc->fileName, nullptr, nullptr,
		Desc->entryPoint, "vs_5_0", CompileFlag,
		0, &ShaderObject, &ShaderErr);

	memcpy(&shaderDescription, Desc, sizeof(FShaderDesc));
}

void CVertexShader::Compile()
{

}
