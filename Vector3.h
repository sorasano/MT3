#pragma once
class Vector3
{
public:
	float x;
	float y;
	float z;

public:
	Vector3();
	Vector3(float x, float y, float z);

	float length() const;
	Vector3& normalize();
	float dot(const Vector3& v)const;
	Vector3 cross(const Vector3& v) const;

	Vector3 operator+() const;
	Vector3 operator-() const;

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
};

const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

//��Ԋ֐�
//���`���(1���֐����)
const Vector3 lerp(Vector3 start, Vector3 end,  float t);

