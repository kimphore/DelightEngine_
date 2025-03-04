#pragma once

#include "Vector.h"
#include "Matrix.h"
#include <DirectXMath.h>

inline Rotation3 DirectionVectorToRotation(Vector3 v)
{
	v.Normalize();

	// Compute Yaw and Pitch
	float yaw = atan2f(v.x, v.z);         // Rotation around Y-axis
	float pitch = -asinf(v.y);          // Rotation around X-axis
	float roll = 0.0f;                // Roll is undefined, set to 0 by default

	return Rotation3(roll, pitch, yaw);
}