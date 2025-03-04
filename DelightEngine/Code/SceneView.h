#pragma once

#include "Include.h"
#include "DelightActor.h"

#include "eastl/vector.h"

struct FSceneInfo
{
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjectionMatrix;
};

class CDelightSceneView
{

public:
	void BackupSceneMatrixInfo();

public:
	void AddActor(CDelightActor* InActor, ESceneActorType InActorType);
	void SetMatrices(Matrix4x4& InViewMatrix, Matrix4x4& InProjectionMatrix);

public:
	void Tick(float32 TimeDelta);

protected:
	eastl::vector<CDelightActor*> TickActors; //  Tick Actors.
	eastl::vector<CDelightActor*> StaticActors; // static actors.

protected:
	FSceneInfo SceneInfo;
	FSceneInfo PrevSceneInfo;
};