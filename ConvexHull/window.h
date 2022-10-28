#pragma once


#include <memory>
#include<mutex>
#include <string>
#include <vector>

#include "SDL.h"

#include "color_rgba.h"
#include "drawing_context.h"
#include "window_eventargs.h"

namespace ch_project
{
	class window
	{
	public:
		window() : window("untitled") {};
		window(const std::string& title) : m_title(title), m_width(1024), m_height(820) {};

		void display();

		void add_dot(const vector2f& dot, const color_rgba& color, const float& scale_x, const float& scale_y);

		void add_dots(const std::vector<vector2f>& dots, const color_rgba& color,
			const float& scale_x, const float& scale_y);

		void clear_dots();

		void add_lines(const std::vector<std::pair<vector2f, vector2f>>& lines, const color_rgba& color,
			const float& scale_x, const float& scale_y);

		void clear_lines();

	protected:
		virtual void handle_event(const SDL_Event& event, window_eventargs& args) {};
		virtual void update(const ch_project::drawing_context& context);

		std::mutex m_mut;
	private:
		std::string m_title;
		std::size_t m_width, m_height;

		std::vector<std::tuple<vector2f, color_rgba, float, float>> m_dots;
		std::vector<std::tuple<vector2f, vector2f, color_rgba, float, float>> m_lines;
	};
}

