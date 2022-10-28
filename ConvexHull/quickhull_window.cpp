#include "quickhull_window.h"


void quickhull_window::set_hull(const std::vector < std::tuple<int, vector2f, vector2f>>& hull)
{
	std::vector<std::pair<vector2f, vector2f>> h;

	for (auto it = hull.begin(); it < hull.end(); ++it)
		h.push_back(std::make_pair(std::get<1>(*it), std::get<2>(*it)));

	std::lock_guard<std::mutex> guard(m_mut);
	m_hull = h;
}

void quickhull_window::set_hull(const std::vector<std::pair<vector2f, vector2f>>& hull)
{
	std::lock_guard<std::mutex> guard(m_mut);
	m_hull = hull;
}

void quickhull_window::handle_event(const SDL_Event& event, ch_project::window_eventargs& args)
{
	switch (event.type)
	{
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

void quickhull_window::update(const ch_project::drawing_context& context)
{
	//Todo: fix that shit
	{
		for (auto it = m_point_cloud.begin(); it < m_point_cloud.end(); ++it)
			context.draw_point(*it, 0, 0, 0, 255, 3.0f, 3.0f);

		std::lock_guard<std::mutex> guard(m_mut);

		for (auto it = m_hull.begin(); it < m_hull.end(); ++it)
			context.draw_line(it->first, it->second, 0, 0, 0, 255, 1.0f, 1.0f);


		/*context.draw_line(vector2f(10.0f, 10.0f), vector2f(1000.0f, 10.0f),
			0, 0, 255, 255, 1.0f, 1.0f);*/

		context.draw_line(vector2f(900.0f, 10.0f), vector2f(1000.0f, 10.0f),
			0, 0, 255, 255, 1.0f, 1.0f);

		context.draw_line(vector2f(800.0f, 20.0f), vector2f(1000.0f, 20.0f),
			0, 0, 255, 255, 1.0f, 1.0f);
	}

	//this too
	ch_project::window::update(context);
}
