#pragma once

#include <numbers>

namespace ch_project
{
	namespace math_helpers
	{
		const double PI = 3.141592653589793238463;
		const float  PI_F = 3.14159265358979f;

		inline float deg_to_rad_f(const float& degree)
		{
			return (degree * PI_F) / 180.0f;
		}

		inline float rad_to_deg_f(const float& rad)
		{
			return rad * (180.0 / PI_F);
		}
	}
}