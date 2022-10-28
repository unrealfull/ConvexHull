#pragma once

#include "vector3f.h"

namespace ch_project
{
	class matrix3f
	{
	public:
		matrix3f(const float& x1, const float& y1, const float& z1,
				const float& x2, const float& y2, const float& z2,
				const float& x3, const float& y3, const float& z3);

		vector3f operator*(const vector3f& right);
	private:
		float m_data[9];

#pragma region layout
		inline float& x_1() { return m_data[0]; };
		inline float& x_2() { return m_data[1]; };
		inline float& x_3() { return m_data[2]; };

		inline float& y_1() { return m_data[3]; };
		inline float& y_2() { return m_data[4]; };
		inline float& y_3() { return m_data[5]; };

		inline float& z_1() { return m_data[6]; };
		inline float& z_2() { return m_data[7]; };
		inline float& z_3() { return m_data[8]; };
#pragma endregion
	};
}

