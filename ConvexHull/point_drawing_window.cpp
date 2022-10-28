#include "point_drawing_window.h"

void point_drawing_window::handle_event(const SDL_Event& event, ch_project::window_eventargs& args)
{
	switch (event.type)
	{
		case SDL_MOUSEBUTTONDOWN:
		{
			switch (event.button.button)
			{
				case SDL_BUTTON_LEFT:
					m_vectors.push_back(vector2f(event.motion.x, event.motion.y));
					break;
				case SDL_BUTTON_RIGHT:
					if (!m_vectors.empty())
						m_vectors.pop_back();
					break;
				default:
					break;
			}

			break;
		}
		case SDL_QUIT:
			args.cancel = true;
			break;
		case SDLK_KP_ENTER:
			args.cancel = true;
			break;
		default:
			break;
	}
}

void point_drawing_window::update(const ch_project::drawing_context& context)
{
	for (std::vector<vector2f>::iterator vec = m_vectors.begin(); vec < m_vectors.end(); ++vec)
		context.draw_point(*vec, 0, 0, 0, 255, 3.0f, 3.0f);
}
