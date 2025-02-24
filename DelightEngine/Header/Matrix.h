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

	Matrix4x4 Transpose() const 
	{
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result.m[i][j] = m[j][i];
			}
		}

		return result;
	}

	Matrix4x4 Inverse() const {
		Matrix4x4 result;
		float inv[16], det;
		float temp[16] = {
			m[0][0], m[0][1], m[0][2], m[0][3],
			m[1][0], m[1][1], m[1][2], m[1][3],
			m[2][0], m[2][1], m[2][2], m[2][3],
			m[3][0], m[3][1], m[3][2], m[3][3]
		};

		inv[0] = temp[5] * temp[10] * temp[15] - temp[5] * temp[11] * temp[14] - temp[9] * temp[6] * temp[15] + temp[9] * temp[7] * temp[14] + temp[13] * temp[6] * temp[11] - temp[13] * temp[7] * temp[10];
		inv[4] = -temp[4] * temp[10] * temp[15] + temp[4] * temp[11] * temp[14] + temp[8] * temp[6] * temp[15] - temp[8] * temp[7] * temp[14] - temp[12] * temp[6] * temp[11] + temp[12] * temp[7] * temp[10];
		inv[8] = temp[4] * temp[9] * temp[15] - temp[4] * temp[11] * temp[13] - temp[8] * temp[5] * temp[15] + temp[8] * temp[7] * temp[13] + temp[12] * temp[5] * temp[11] - temp[12] * temp[7] * temp[9];
		inv[12] = -temp[4] * temp[9] * temp[14] + temp[4] * temp[10] * temp[13] + temp[8] * temp[5] * temp[14] - temp[8] * temp[6] * temp[13] - temp[12] * temp[5] * temp[10] + temp[12] * temp[6] * temp[9];

		det = temp[0] * inv[0] + temp[1] * inv[4] + temp[2] * inv[8] + temp[3] * inv[12];
		if (det == 0) return *this;

		det = 1.0f / det;
		for (int i = 0; i < 16; i++)
		{
			inv[i] *= det;
		}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.m[i][j] = inv[i * 4 + j];
			}
		}

		return result;
	}
	// row major..
	void Transform(Vector3& InPosition)
	{
		Identity();
		m[0][3] = InPosition.x;
		m[1][3] = InPosition.y;
		m[2][3] = InPosition.z;
	}

	void Scale(Vector3& InScale)
	{
		Identity();
		m[0][0] = InScale.x;
		m[1][1] = InScale.x;
		m[2][2] = InScale.x;
	}

	void Rotation(Rotation3& InRotation)
	{
		float cx = cos(InRotation.roll), sx = sin(InRotation.roll);
		float cy = cos(InRotation.pitch), sy = sin(InRotation.pitch);
		float cz = cos(InRotation.yaw), sz = sin(InRotation.yaw);

		m[0][0] = cy * cz;
		m[0][1] = cz * sx * sy - cx * sz;
		m[0][2] = cx * cz * sy + sx * sz;
		m[0][3] = 0.0f;

		m[1][0] = cy * sz;
		m[1][1] = cx * cz + sx * sy * sz;
		m[1][2] = -cz * sx + cx * sy * sz;
		m[1][3] = 0.0f;

		m[2][0] = -sy;
		m[2][1] = cy * sx;
		m[2][2] = cx * cy;
		m[2][3] = 0.0f;

		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
		m[3][2] = 0.0f;
		m[3][3] = 1.0f;
	}

	void Rotation_XAxis(float32 Roll)
	{
		Identity();
		float c = cos(Roll);
		float s = sin(Roll);

		m[1][1] = c;
		m[1][2] = -s;
		m[2][1] = s;
		m[2][2] = c;
	}

	void Rotation_YAxis(float32 Pitch)
	{
		Identity();
		float c = cos(Pitch);
		float s = sin(Pitch);

		m[0][0] = c;
		m[0][2] = s;
		m[2][0] = -s;
		m[2][2] = c;
	}

	void Rotation_ZAxis(float32 Yaw)
	{
		Identity();
		float c = cos(Yaw);
		float s = sin(Yaw);

		m[0][0] = c;
		m[0][1] = -s;
		m[1][0] = s;
		m[1][1] = c;
	}
};