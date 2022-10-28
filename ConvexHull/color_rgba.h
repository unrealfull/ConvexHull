#pragma once

#include "SDL.h"

namespace ch_project
{
	struct color_rgba
	{
		Uint8 r, g, b, a;

		color_rgba() :r(0), g(0), b(0), a(255) {};
		color_rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a) :r(r), g(g), b(b), a(a) {};
	};

	namespace color
	{
		inline color_rgba red() { return color_rgba(255, 0, 0, 255); };
		inline color_rgba green() { return color_rgba(0, 255, 0, 255); };
		inline color_rgba blue() { return color_rgba(0, 0, 255, 255); };
	}
}