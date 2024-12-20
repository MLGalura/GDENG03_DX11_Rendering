#pragma once

class Vector3D 
{
public:
	Vector3D() : m_x(0), m_y(0), m_z(0) {

	}
	
	Vector3D(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {

	}
	
	Vector3D(const Vector3D& vector	) : m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z) {

	}

	static Vector3D lerp(const Vector3D& start, const Vector3D& end, float delta) {
		Vector3D v;

		v.m_x = start.m_x * (1.0f - delta) + end.m_x * (delta);
		v.m_y = start.m_y * (1.0f - delta) + end.m_y * (delta);
		v.m_z = start.m_z * (1.0f - delta) + end.m_z * (delta);

		return v;
	}

	static Vector3D Zero() {
		return Vector3D(0.0f, 0.0f, 0.0f);
	}
	
	static Vector3D One() {
		return Vector3D(1.0f, 1.0f, 1.0f);
	}

	Vector3D operator *(float num)
	{
		return Vector3D(this->m_x * num, this->m_y * num, this->m_z * num);
	}
	
	Vector3D operator +(Vector3D vec)
	{
		return Vector3D(this->m_x + vec.m_x, this->m_y + vec.m_y, this->m_z + vec.m_z);
	}

	~Vector3D() {

	}

public:
	float m_x, m_y, m_z;
};