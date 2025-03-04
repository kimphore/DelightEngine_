#include "DelightCameraMgr.h"
#include "DelightCameraActor.h"

CDelightCameraManager GCameraManager;

Matrix4x4 CDelightCameraManager::GetViewMatrix()
{
	Matrix4x4 DefaultMatrix;

	if (CurrentCamera)
	{
		return CurrentCamera->GetProjectionMatrix();
	}

	return DefaultMatrix;
}

Matrix4x4 CDelightCameraManager::GetProjectionMatrix()
{
	Matrix4x4 DefaultMatrix;

	if (CurrentCamera)
	{
		return CurrentCamera->GetViewMatrix();
	}
	
	return DefaultMatrix;
}
