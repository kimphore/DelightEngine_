#pragma once

#include "Include.h"

#include "eastl/vector.h"

class CDelightCameraActor;
class ENGINE_DLL CDelightCameraManager
{
public:
	Matrix4x4 GetViewMatrix();
	Matrix4x4 GetProjectionMatrix();

public:
	void BindCamera(CDelightCameraActor* InCamera) {
		CurrentCamera = InCamera;
	}

private:
	CDelightCameraActor* CurrentCamera;
	eastl::vector<CDelightCameraActor*> CameraActors;
};

extern ENGINE_DLL CDelightCameraManager GCameraManager;