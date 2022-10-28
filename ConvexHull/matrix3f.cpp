#include "matrix3f.h"

ch_project::matrix3f::matrix3f(const float& x1, const float& y1, const float& z1, const float& x2, const float& y2, const float& z2, const float& x3, const float& y3, const float& z3)
{
	x_1() = x1;
	x_2() = x2;
	x_3() = x3;

	y_1() = y1;
	y_2() = y2;
	y_3() = y3;

	z_1() = z1;
	z_2() = z2;
	z_3() = z3;
}

ch_project::vector3f ch_project::matrix3f::operator*(const vector3f& right)
{
	float x = (x_1() * right.X() + y_1() * right.Y() + z_1() * right.Z());
	float y = (x_2() * right.X() + y_2() * right.Y() + z_2() * right.Z());
	float z = (x_3() * right.X() + y_3() * right.Y() + z_3() * right.Z());

	return vector3f(x, y, z);
}
