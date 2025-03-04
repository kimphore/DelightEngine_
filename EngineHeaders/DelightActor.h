#pragma once

#include "Include.h"

class ENGINE_DLL CDelightActor
{
public:
	CDelightActor() {};
public:
	virtual void Tick(float InDeltaTime) {};

public:
	void SetTransform(Vector3& _Position, Rotation3& _Rotation, Vector3& _Scale);
protected:
	Vector3 Position;
	Rotation3 Rotation; // roll pitch yaw
	Vector3 Scale;

	Matrix4x4 WorldMatrix;
};