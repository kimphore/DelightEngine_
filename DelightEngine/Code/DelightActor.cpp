#include "DelightActor.h"

void CDelightActor::SetTransform(Vector3& _Position, Rotation3& _Rotation, Vector3& _Scale)
{
	Position = _Position;
	Rotation = _Rotation;
	Scale = _Scale;

	Matrix4x4 TransformMatrix, RotationMatrix, ScaleMatrix;

	TransformMatrix.Transform(Position);
	RotationMatrix.Rotation(Rotation);
	ScaleMatrix.Scale(Scale);

	WorldMatrix = ScaleMatrix * RotationMatrix * TransformMatrix;
}
