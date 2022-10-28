#include "window.h"

void ch_project::window::display()
{
	bool quit = false;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);

	{
		std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window(
			SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, 0),
			[](SDL_Window* win) { SDL_DestroyWindow(win); }
		);

		std::shared_ptr<SDL_Renderer> renderer(
			SDL_CreateRenderer(window.get(), -1, 0),
			[](SDL_Renderer* ren) { SDL_DestroyRenderer(ren); }
		);

		ch_project::drawing_context ctx(renderer);

		while (true)
		{
			SDL_Delay(10);
			SDL_PollEvent(&event);

			window_eventargs args;
			handle_event(event, args);

			if (args.cancel)
				break;

			SDL_SetRenderDrawColor(renderer.get(), 242, 242, 242, 255);
			SDL_RenderClear(renderer.get());

			update(ctx);
			SDL_RenderPresent(renderer.get());
		}
	}

	SDL_Quit();
}

void ch_project::window::add_dot(const vector2f& dot, const color_rgba& color, const float& scale_x, const float& scale_y)
{
	std::lock_guard<std::mutex> guard(m_mut);

	m_dots.push_back(std::tuple<vector2f, color_rgba, float,float>(dot, color, scale_x, scale_y));
}

void ch_project::window::add_dots(const std::vector<vector2f>& dots, const color_rgba& color,
	const float& scale_x, const float& scale_y)
{
	std::lock_guard<std::mutex> guard(m_mut);

	for (auto it = dots.begin(); it < dots.end(); ++it)
		m_dots.push_back(std::tuple<vector2f, color_rgba, float, float>(*it, color, scale_x, scale_y));
}

void ch_project::window::clear_dots()
{
	std::lock_guard<std::mutex> guard(m_mut);

	m_dots.clear();
}

void ch_project::window::add_lines(const std::vector<std::pair<vector2f, vector2f>>& lines, const color_rgba& color, const float& scale_x, const float& scale_y)
{
	std::lock_guard<std::mutex> guard(m_mut);

	for (auto it = lines.begin(); it < lines.end(); ++it)
		m_lines.push_back(std::tuple<vector2f, vector2f, color_rgba, float, float>(
			it->first, it->second, color, scale_x, scale_y
			));
}

void ch_project::window::clear_lines()
{
	std::lock_guard<std::mutex> guard(m_mut);

	m_lines.clear();
}

void ch_project::window::update(const ch_project::drawing_context& context)
{
	std::lock_guard<std::mutex> guard(m_mut);

	for (auto it = m_dots.begin(); it < m_dots.end(); ++it)
	{
		const vector2f& v = std::get<0>(*it);
		const color_rgba& c = std::get<1>(*it);
		context.draw_point(v, c.r, c.g, c.b, c.a, std::get<2>(*it), std::get<3>(*it));
	}

	for (auto it = m_lines.begin(); it < m_lines.end(); ++it)
	{
		const vector2f& from = std::get<0>(*it);
		const vector2f& to = std::get<1>(*it);
		const color_rgba& c = std::get<2>(*it);

		context.draw_line(from, to, c.r, c.g, c.b, c.a, std::get<3>(*it), std::get<4>(*it));
	}
}
