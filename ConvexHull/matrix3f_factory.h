#pragma once

#include "matrix3f.h"
#include "vector2f.h"

namespace ch_project
{
	namespace prebuilt
	{
		matrix3f homogenized_translate(const vector2f& translation_vector)
		{
			return matrix3f(
				1.0f, 0.0f, translation_vector.X(),
				0.0f, 1.0f, translation_vector.Y(),
				0.0f, 0.0f, 1.0f
			);
		}
	}
}