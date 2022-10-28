#pragma once

#include <math.h>

#include "vector2f.h"

namespace ch_project
{
	class vector3f
	{
	public:
		float X() const { return m_x; };
		float Y() const { return m_y; };
		float Z() const { return m_z; };

		vector3f() : m_x(0), m_y(0), m_z(0) {};
		vector3f(const float& x, const float& y, const float& z) : m_x(x), m_y(y), m_z(z) {};

		vector3f cross(const vector3f& right) const;
		float norm() const;

		explicit operator vector2f() const;

		vector3f operator -(const vector3f& right) const;
	private:
		float m_x, m_y, m_z;
	};
}
