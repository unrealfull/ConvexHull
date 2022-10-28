#pragma once


#include<vector>

#include "node.h"

#include "vector2f.h"
#include "window.h"

class quickhull_window : public ch_project::window
{
public:
	/*void set_hull();*/
	void set_hull(const std::vector < std::tuple<int, vector2f, vector2f>>& hull);
	void set_hull(const std::vector<std::pair<vector2f, vector2f>>& hull);

	quickhull_window(const std::string& title, const std::vector<vector2f>& point_cloud)
		: ch_project::window(title), m_point_cloud(point_cloud) {};

protected:
	void handle_event(const SDL_Event& event, ch_project::window_eventargs& args) override;
	void update(const ch_project::drawing_context& context) override;
private:
	std::vector<std::pair<vector2f, vector2f>> m_hull;
	std::vector<vector2f> m_point_cloud;
};

