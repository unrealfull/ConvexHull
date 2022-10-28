#pragma once

#include <memory>

#include "SDL.h"
#include "vector2f.h"


namespace ch_project
{
	class drawing_context
	{
	public:
		drawing_context(const std::shared_ptr<SDL_Renderer>& renderer)
			: m_renderer(renderer) {};

		void draw_point(const vector2f& position_vector,
			const Uint8& r, const Uint8& g, const Uint8& b, const Uint8& a,
			const float& scale_x, const float& scale_y) const;

		void draw_line(const vector2f& from, const vector2f& to,
			const Uint8& r, const Uint8& g, const Uint8& b, const Uint8& a,
			const float& scale_x, const float& scale_y) const;
	private:
		std::shared_ptr<SDL_Renderer> m_renderer;
	};
}
