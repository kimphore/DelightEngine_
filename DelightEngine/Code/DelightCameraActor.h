#pragma once

#include "DelightActor.h"

class CDelightCameraActor : CDelightActor
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

protected:
	void UpdateMatrix();
protected:
	bool8 bCurrentCamera = false;
	Matrix4x4 ViewMatrix;

	Vector3 TargetVector;
	Vector3 UpVector;
	Vector3 ForwardVector;
	Vector3 RightVector;
};