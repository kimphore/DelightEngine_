#include "DelightCameraActor.h"
#include <DirectXMath.h>

CDelightCameraActor::CDelightCameraActor()
{
	Position = Vector3(0.f, 0.f, 0.f);
	TargetVector = Vector3(100.f, 0.f, 0.f);
	UpVector = Vector3(0.f, 0.f, 1.f);
	RightVector = Vector3(1.f, 0.f, 0.f);

	UpdateMatrix();
}

void CDelightCameraActor::Tick(float InDeltaTime)
{

}

void CDelightCameraActor::SetUseCamera(bool bUse)
{
	bUse = true;
}

void CDelightCameraActor::UpdateMatrix()
{
	DirectX::XMVECTOR Pos = Position.ToXMVector();
	DirectX::XMVECTOR Forward = ForwardVector.ToXMVector();
	DirectX::XMVECTOR Up = UpVector.ToXMVector();
	DirectX::XMVECTOR Right = RightVector.ToXMVector();

	Forward = DirectX::XMVector3Normalize(Forward);
	Right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(Up, Forward));
	Up = DirectX::XMVector3Cross(Forward, Right);

	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(Pos, Right));
	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(Pos, Up));
	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(Pos, Forward));

	ViewMatrix.m[0][0] = DirectX::XMVectorGetX(Right);
	ViewMatrix.m[0][1] = DirectX::XMVectorGetX(Right);
	ViewMatrix.m[0][2] = DirectX::XMVectorGetX(Right);
	ViewMatrix.m[0][3] = x;

	ViewMatrix.m[1][0] = DirectX::XMVectorGetX(Up);
	ViewMatrix.m[1][1] = DirectX::XMVectorGetX(Up);
	ViewMatrix.m[1][2] = DirectX::XMVectorGetX(Up);
	ViewMatrix.m[1][3] = y;

	ViewMatrix.m[2][0] = DirectX::XMVectorGetX(Forward);
	ViewMatrix.m[2][1] = DirectX::XMVectorGetX(Forward);
	ViewMatrix.m[2][2] = DirectX::XMVectorGetX(Forward);
	ViewMatrix.m[2][3] = z;

	ViewMatrix.m[3][0] = 0.f;
	ViewMatrix.m[3][1] = 0.f;
	ViewMatrix.m[3][2] = 0.f;
	ViewMatrix.m[3][3] = 1.f;
}
