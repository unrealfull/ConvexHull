#include "drawing_context.h"


void ch_project::drawing_context::draw_point(const vector2f& position_vector, const Uint8& r, const Uint8& g, const Uint8& b, const Uint8& a, const float& scale_x, const float& scale_y) const
{
	SDL_SetRenderDrawColor(m_renderer.get(), r, g, b, a);
	SDL_RenderSetScale(m_renderer.get(), scale_x, scale_y);
	SDL_RenderDrawPointF(m_renderer.get(), position_vector.X() / scale_x, position_vector.Y() / scale_y);
}

void ch_project::drawing_context::draw_line(const vector2f& from, const vector2f& to, const Uint8& r, const Uint8& g, const Uint8& b, const Uint8& a, const float& scale_x, const float& scale_y) const
{
	SDL_SetRenderDrawColor(m_renderer.get(), r, g, b, a);
	SDL_RenderSetScale(m_renderer.get(), scale_x, scale_y);
	SDL_RenderDrawLineF(m_renderer.get(), from.X(), from.Y(), to.X(), to.Y());
}
