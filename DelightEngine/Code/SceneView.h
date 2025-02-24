#pragma once

#include "Include.h"

struct FSceneMatrixInfo
{
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjectionMatrix;
};

class CDelightSceneView
{
protected:
	FSceneMatrixInfo MatrixSceneInfo;
	FSceneMatrixInfo PrevMatrixSceneInfo;
};