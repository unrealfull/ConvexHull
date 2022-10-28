#pragma once

#include <math.h>

class vector2f
{
public:
	const float& X() const { return m_x; };
	const float& Y() const { return m_y; };

	vector2f() : m_x(0.0f), m_y(0.0f) {};
	vector2f(const float& x, const float& y) : m_x(x), m_y(y) {}

	vector2f operator+(const vector2f& right) const;
	vector2f operator-(const vector2f& right) const;
	vector2f operator*(const float& scalar) const;

	float cross(const vector2f& right) const;
	float dot(const vector2f& right) const;
	float norm() const;
	vector2f normalized() const;
	float angle_r(const vector2f& right) const;
private:
	float m_x, m_y;
};

