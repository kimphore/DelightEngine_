#pragma once

#include "Typedef.h"

#include <cmath>
#include <DirectXMath.h>
/*
*	 저장용 Vector / 연산용 XMVector..?
*/

struct Vector2
{
	float x;
	float y;

	Vector2() {}
	Vector2(float _x, float _y)
		:x(_x), y(_y)
	{
	}

	Vector2 operator +(const Vector2& other) const {
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 operator -(const Vector2& other) const {
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 operator / (const float& other) const {
		return Vector2(x / other, y / other);
	}

	Vector2 operator *(const float& other) const {
		return Vector2(x * other, y * other);
	}

	Vector2& operator +=(const Vector2& other) {
		x += other.x;
		y += other.y;

		return *this;
	}

	Vector2& operator -=(const Vector2& other) {
		x -= other.x;
		y -= other.y;

		return *this;
	}

	Vector2& operator *=(const Vector2& other) {
		x *= other.x;
		y *= other.y;

		return *this;
	}

	Vector2& operator /=(const Vector2& other) {
		x /= other.x;
		y /= other.y;

		return *this;
	}
};

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3() {}
	Vector3(float _x, float _y, float _z)
		:x(_x), y(_y), z(_z)
	{
	}

	Vector3 operator +(const Vector3& other) const {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 operator -(const Vector3& other) const {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 operator / (const float& other) const {
		return Vector3(x / other, y / other, z / other);
	}

	Vector3 operator *(const float& other) const {
		return Vector3(x * other, y * other, z * other);
	}

	Vector3& operator +=(const Vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	Vector3& operator -=(const Vector3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	Vector3& operator *=(const Vector3& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;

		return *this;
	}

	Vector3& operator /=(const Vector3& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;

		return *this;
	}

	DirectX::XMVECTOR ToXMVector()
	{
		return DirectX::XMVectorSet(x, y, z, 0);
	}

	void Normalize()
	{
		float Size = x * x + y * y + z * z;
		Size = sqrt(Size);

		x /= Size;
		y /= Size;
		z /= Size;
	}
};

struct Vector4
{
	float x;
	float y;
	float z;
	float w;

	Vector4() {}
	Vector4(float _x, float _y, float _z, float _w)
		:x(_x), y(_y), z(_z), w(_w)
	{
	}

	Vector4 operator +(const Vector4& other) const {
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Vector4 operator -(const Vector4& other) const {
		return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	Vector4 operator / (const float& other) const {
		return Vector4(x / other, y / other, z / other, w / other);
	}

	Vector4 operator *(const float& other) const {
		return Vector4(x * other, y * other, z * other, w * other);
	}

	Vector4& operator +=(const Vector4& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}

	Vector4& operator -=(const Vector4& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}

	Vector4& operator *=(const Vector4& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;

		return *this;
	}

	Vector4& operator /=(const Vector4& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;

		return *this;
	}
};

struct Rotation3
{
	float32 roll; // z, x axis rotation
	float32 pitch; // y, y axis rotation
	float32 yaw; // x, z axis rotation

	Rotation3(){}
	Rotation3(float32 _roll, float32 _pitch, float32 _yaw)
		:roll(_roll), pitch(_pitch), yaw(_yaw)
	{
	}
};