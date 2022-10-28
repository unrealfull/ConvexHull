#include "vector2f.h"

vector2f vector2f::operator+(const vector2f& right) const
{
	return vector2f(m_x + right.m_x, m_y + right.m_y);
}

vector2f vector2f::operator-(const vector2f& right) const
{
	return vector2f(m_x - right.m_x, m_y - right.m_y);
}

vector2f vector2f::operator*(const float& scalar) const
{
	return vector2f(m_x * scalar, m_y * scalar);
}

float vector2f::cross(const vector2f& right) const
{
	return (m_x * right.m_y) - (m_y * right.m_x);
}

float vector2f::dot(const vector2f& right) const
{
	return m_x * right.m_x + m_y * right.m_y;
}

float vector2f::norm() const
{
	float len = pow(m_x, 2.0f) + pow(m_y, 2.0f);
	return sqrt(len);
}

vector2f vector2f::normalized() const
{
	return (*this) * (1 / norm());
}

float vector2f::angle_r(const vector2f& right) const
{
	return acos(dot(right));
}
