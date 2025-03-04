#pragma once

#include "DelightActor.h"

class ENGINE_DLL CDelightCameraActor : CDelightActor
{
public:
	CDelightCameraActor();

public:
	virtual void Tick(float InDeltaTime) override;

public:
	void SetUseCamera(bool bUse);
public:
	Matrix4x4& GetViewMatrix() {
		return ViewMatrix;
	}
	Matrix4x4& GetProjectionMatrix() {
		return ProjectionMatrix;
	}

protected:
	void UpdateMatrix();
protected:
	bool8 bCurrentCamera = false;
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjectionMatrix;

	Vector3 TargetVector;
	Vector3 UpVector;
	Vector3 ForwardVector;
	Vector3 RightVector;
};