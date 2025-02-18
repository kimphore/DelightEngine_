#pragma once
#include "Typedef.h"

namespace Delight
{
	struct FMD5Hash
	{
		byte hash[128];

		uint32 GetSize() {
			return 128;
		}

		FMD5Hash()
		{
			memset(hash, 0, sizeof(byte) * 128);
		}
	};
}

struct FInt32_2
{
	int32 x = 0;
	int32 y = 0;

	FInt32_2() {}
	FInt32_2(int32 inX, int32 inY)
		:x(inX), y(inY)
	{
	}
};

struct FInt32_3
{
	int32 x = 0;
	int32 y = 0;
	int32 z = 0;

	FInt32_3() {}
	FInt32_3(int32 inX, int32 inY, int32 inZ)
		:x(inX), y(inY), z(inZ)
	{
	}
};

struct FFloat2
{
	float x;
	float y;

	FFloat2() {}
	FFloat2(float _x, float _y)
		:x(_x), y(_y)
	{}

	FFloat2 operator +(const FFloat2& other) const {
		return FFloat2(x + other.x, y + other.y);
	}

	FFloat2 operator -(const FFloat2& other) const {
		return FFloat2(x - other.x, y - other.y);
	}

	FFloat2 operator / (const float& other) const {
		return FFloat2(x / other, y / other);
	}

	FFloat2 operator *(const float& other) const {
		return FFloat2(x * other, y * other);
	}

	FFloat2& operator +=(const FFloat2& other) {
		x += other.x;
		y += other.y;

		return *this;
	}

	FFloat2& operator -=(const FFloat2& other) {
		x -= other.x;
		y -= other.y;

		return *this;
	}

	FFloat2& operator *=(const FFloat2& other) {
		x *= other.x;
		y *= other.y;

		return *this;
	}

	FFloat2& operator /=(const FFloat2& other) {
		x /= other.x;
		y /= other.y;

		return *this;
	}
};

struct FFloat3
{
	float x;
	float y;
	float z;

	FFloat3() {}
	FFloat3(float _x, float _y, float _z)
		:x(_x), y(_y), z(_z)
	{}

	FFloat3 operator +(const FFloat3& other) const {
		return FFloat3(x + other.x, y + other.y, z + other.z);
	}

	FFloat3 operator -(const FFloat3& other) const {
		return FFloat3(x - other.x, y - other.y, z - other.z);
	}

	FFloat3 operator / (const float& other) const {
		return FFloat3(x / other, y / other, z / other);
	}

	FFloat3 operator *(const float& other) const {
		return FFloat3(x * other, y * other, z * other);
	}

	FFloat3& operator +=(const FFloat3& other) {
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	FFloat3& operator -=(const FFloat3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	FFloat3& operator *=(const FFloat3& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;

		return *this;
	}

	FFloat3& operator /=(const FFloat3& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;

		return *this;
	}
};

struct FFloat4
{
	float x;
	float y;
	float z;
	float w;

	FFloat4() {}
	FFloat4(float _x, float _y, float _z, float _w)
		:x(_x), y(_y), z(_z), w(_w)
	{}

	FFloat4 operator +(const FFloat4& other) const {
		return FFloat4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	FFloat4 operator -(const FFloat4& other) const {
		return FFloat4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	FFloat4 operator / (const float& other) const {
		return FFloat4(x / other, y / other, z / other, w / other);
	}

	FFloat4 operator *(const float& other) const {
		return FFloat4(x * other, y * other, z * other, w * other);
	}

	FFloat4& operator +=(const FFloat4& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}

	FFloat4& operator -=(const FFloat4& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}

	FFloat4& operator *=(const FFloat4& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;

		return *this;
	}

	FFloat4& operator /=(const FFloat4& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;

		return *this;
	}
};