#pragma once

#include "vector2f.h"
#include "vector3f.h"

namespace ch_project
{
	namespace conversions
	{
		vector3f homogenized_vector(const vector2f& value) { return ch_project::vector3f(value.X(), value.Y(), 0); };
		vector3f homogenized_position(const vector2f& value) { return ch_project::vector3f(value.X(), value.Y(), 1); };
	}
}
