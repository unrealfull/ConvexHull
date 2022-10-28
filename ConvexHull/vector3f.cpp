#include "vector3f.h"

ch_project::vector3f ch_project::vector3f::cross(const vector3f& right) const
{
	return vector3f(
		m_y * right.m_z - m_z * right.m_y,
		m_z * right.m_x - m_x * right.m_z,
		m_x * right.m_y - m_y * right.m_x
	);
}

float ch_project::vector3f::norm() const
{
	return sqrt(pow(m_x, 2) + pow(m_y, 2) + pow(m_z, 2));
}

ch_project::vector3f::operator vector2f() const
{
	return vector2f(m_x, m_y);
}

ch_project::vector3f ch_project::vector3f::operator-(const vector3f& right) const
{
	return vector3f(
		m_x - right.m_x,
		m_y - right.m_y,
		m_z - right.m_z
	);
}
