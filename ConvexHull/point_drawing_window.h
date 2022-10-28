#pragma once

#include <iostream>
#include <functional>
#include <vector>

#include "SDL.h"
#include "vector2f.h"
#include "window.h"

class point_drawing_window : public ch_project::window
{
public:
	point_drawing_window(const std::string& title) : ch_project::window(title) {  };

	std::vector<vector2f> get_vectors() const { return m_vectors; };
private:
	std::vector<vector2f> m_vectors;
protected:
	void handle_event(const SDL_Event& event, ch_project::window_eventargs& args) override;
	void update(const ch_project::drawing_context& context) override;
};

