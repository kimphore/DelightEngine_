#include "SceneView.h"

void CDelightSceneView::BackupSceneMatrixInfo()
{
	PrevSceneInfo.ViewMatrix = SceneInfo.ViewMatrix;
	PrevSceneInfo.ProjectionMatrix = SceneInfo.ProjectionMatrix;
}

void CDelightSceneView::AddActor(CDelightActor* InActor, ESceneActorType InActorType)
{
	switch (InActorType)
	{
	case SAT_Tickable:
		TickActors.push_back(InActor);
		break;
	case SAT_Static:
		StaticActors.push_back(InActor);
		break;
	}
}

void CDelightSceneView::SetMatrices(Matrix4x4& InViewMatrix, Matrix4x4& InProjectionMatrix)
{
	SceneInfo.ViewMatrix = InViewMatrix;
	SceneInfo.ProjectionMatrix = InProjectionMatrix;
}

void CDelightSceneView::Tick(float32 TimeDelta)
{
	
}
