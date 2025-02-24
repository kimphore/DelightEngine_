#pragma once

#include "Typedef.h"
#include "Vector.h"

#include <DirectXMath.h>

struct Matrix4x4
{
	float32 m[4][4];

	Matrix4x4()
	{}

	void Identity()
	{
		for (int32 i = 0; i < 4; ++i)
		{
			for (int32 j = 0; j < 4; ++j)
			{
				m[i][j] = i == j ? 1.f : 0.f;
			}
		}
	}

	Matrix4x4 operator * (const Matrix4x4& InMatrix) const
	{
		Matrix4x4 Result;
		for (int32 i = 0; i < 4; ++i)
		{
			for (int32 j = 0; j < 4; ++j)
			{
				Result.m[i][j] = 0;
				for (int32 k = 0; k < 4; ++k)
				{
					Result.m[i][j] += m[i][k] * InMatrix.m[k][j];
				}
			}
		}

		return Result;
	}

	Vector4 TransformVector(const Vector4& v)
	{
		return {
			m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
			m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
			m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
			m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
		};
	}

};