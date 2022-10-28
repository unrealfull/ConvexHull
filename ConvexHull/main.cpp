// ConvexHull.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <thread>
#include <vector>
#include <string.h>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include <windows.h>
#include <chrono>
#include<iomanip>
#include "conversions.h"

#include "point_drawing_window.h"
#include "quickhull_window.h"

#include "matrix3f.h"
#include "matrix3f_factory.h"

#include "math_helpers.h"
#include "main.h"

#pragma region input data

// random points
std::vector<vector2f> generate_random_points()
{
	int point_cloud_size;
	std::vector<vector2f> point_cloud;

	std::cout << "\n  How many points? ";
	std::cin >> point_cloud_size;

	// Generate a different random result for rand()
	srand(time(NULL));

	// save points in vector
	for (int i = 0; i < point_cloud_size; i++) {

		// get two random float numbers 
		float x = 10 + static_cast<float>(rand()) * static_cast<float>(1000 - 0) / RAND_MAX;
		float y = 10 + static_cast<float>(rand()) * static_cast<float>(800 - 0) / RAND_MAX;

		// add to vector and point cloud
		vector2f tmp(x, y);
		point_cloud.push_back(tmp);
	}

	//for (vector2f i : point_cloud)
	//		std::cout << "\n" << i.X() << "," << i.Y();

	return point_cloud;
}

// file import
std::vector<vector2f> import_file()
{
	int point_cloud_size;
	std::vector<vector2f> point_cloud;

	// file dialog
	OPENFILENAME ofn;
	TCHAR szFile[260] = { 0 };
	char fileName[MAX_PATH] = "";
	HANDLE hf;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	GetOpenFileName(&ofn);

	//std::cout << szFile << std::endl;

	// Open a file in read mode
	std::fstream inFile;
	inFile.open(szFile);

	inFile >> point_cloud_size;

	// tmp variables to save line and splitted points
	std::string line;
	std::string word;

	// read, split and save for each line x and y
	for (int i = 0; i < point_cloud_size; i++)
	{
		inFile >> line;

		// used for breaking words
		std::stringstream s(line);

		// x
		std::getline(s, word, ',');
		float x = stof(word);

		// y
		std::getline(s, word);
		float y = stof(word);

		// add to vector and point cloud
		vector2f tmp(x, y);
		point_cloud.push_back(tmp);
	}

	//for (vector2f i : point_cloud)
	//	std::cout << "\n" << i.X() << "," << i.Y();

	// Close the file
	inFile.close();

	return point_cloud;
}

// draw points
std::vector<vector2f> draw_points()
{
	std::vector<vector2f> point_cloud;

	point_drawing_window point_drawing("Point Drawing");
	point_drawing.display();
	point_cloud = point_drawing.get_vectors();

	//for (vector2f i : point_cloud)
	//	std::cout << "\n" << i.X() << "," << i.Y();

	return point_cloud;
}

// save points to file
void save_points_to_file(std::vector<vector2f> point_cloud) {

	OPENFILENAME ofn;
	TCHAR szFile[260] = { 0 };
	char fileName[MAX_PATH] = "";
	HANDLE hf;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	GetSaveFileName(&ofn);

	// File pointer
	std::ofstream outFile;

	// creates a new file
	outFile.open(szFile, std::ios::out);

	// Save number of points in first line
	outFile << point_cloud.size() << std::endl;

	// save points in file
	for (vector2f i : point_cloud)
		outFile << i.X() << "," << i.Y() << std::endl;

	// Close the file
	outFile.close();
}

#pragma endregion

#pragma region quickhull

int g_id = 0;
int quckhull_delay = 500;

// quickhull simulation
void remove_from_hull(std::vector<std::tuple<int, vector2f, vector2f>>& hull, const int& id)
{
	if (hull.empty())
		return;

	auto it = std::find_if(hull.begin(), hull.end(),
		[&id](const std::tuple<int, vector2f, vector2f>& item) {return std::get<0>(item) == id; });

	/*std::cout << "remove: " << remove << std::endl;
	std::cout << "it: " << std::get<0>(*it) << std::endl;*/

	if (it != hull.end())
		hull.erase(it);
}

void find_hull_simulation(const std::vector<vector2f>& remaining_data, const vector2f& from, const vector2f& to,
	std::vector<std::tuple<int ,vector2f, vector2f>>& hull, std::shared_ptr<quickhull_window>& window,
	const ch_project::color_rgba& helpline_color, const int& remove)
{
	if (remaining_data.empty())
		return;

	// distanz-abfrage
	// geht so auch:
	/*vector2f separation_line = to - from;

	std::pair<float, std::size_t> d_largest = std::make_pair(0.0f, 0);

	for (auto it = remaining_data.begin(); it < remaining_data.end(); ++it)
	{
		ch_project::vector3f p = ch_project::conversions::homogenized_vector(*it);
		ch_project::vector3f a = ch_project::conversions::homogenized_vector(to);
		ch_project::vector3f b = ch_project::conversions::homogenized_vector(separation_line);

		float d = ((p - a).cross(b)).norm() / b.norm();

		if (d_largest.first < d)
			d_largest = std::make_pair(d, it - remaining_data.begin());
	}

	window->add_dot(remaining_data[d_largest.second], ch_project::color_rgba(255, 0, 0, 255), 6.0f, 6.0f);*/


	/*Dreiecke für die GUI*/
	std::vector<std::pair<vector2f, vector2f>> helplines;

	vector2f separation_line = to - from;
	vector2f separation_line_normalized = separation_line.normalized();
	ch_project::matrix3f t_relative_to_separation_line = ch_project::prebuilt::homogenized_translate(from * (-1));

	std::pair<float, std::size_t> hull_candidate = std::make_pair(0, 0);

	for (auto it = remaining_data.begin(); it < remaining_data.end(); ++it)
	{
		vector2f v_hyp = (t_relative_to_separation_line * ch_project::conversions::homogenized_position(*it)).operator vector2f();

		vector2f hyp_normalized = v_hyp.normalized();
		float hyp = v_hyp.norm();
		float cosine = cos(hyp_normalized.angle_r(separation_line_normalized));
		float adjacent = cosine * hyp;

		vector2f normal(separation_line_normalized.Y(), separation_line_normalized.X() * (-1));
		float opposite = sqrt(pow(hyp, 2) - pow(adjacent, 2));

		vector2f hl_from = from + (separation_line_normalized * adjacent);
		vector2f hl_to = hl_from + (normal * opposite);

		helplines.push_back(std::make_pair(hl_from, hl_to));

		if (hull_candidate.first < opposite)
			hull_candidate = std::make_pair(opposite, it - remaining_data.begin());
	}

	vector2f c = remaining_data[hull_candidate.second];

	window->add_lines(helplines, helpline_color, 1.0f, 1.0f);
	window->add_dot(c, ch_project::color::red(), 6.0f, 6.0f);
	
	if (remove > -1)
		remove_from_hull(hull, remove);

	std::tuple<int, vector2f, vector2f> r(++g_id, from, c);
	hull.push_back(r);

	std::tuple<int, vector2f, vector2f> l(++g_id, c, to);
	hull.push_back(l);

	SDL_Delay(quckhull_delay);
	window->clear_dots();
	window->set_hull(hull);
	SDL_Delay(quckhull_delay);
	window->clear_lines();

	vector2f p_to_c = c - from;
	std::vector<vector2f> s1;

	vector2f c_to_q = to - c;
	std::vector<vector2f> s2;

	ch_project::matrix3f t_relative_p_to_c = ch_project::prebuilt::homogenized_translate(from * (-1));
	ch_project::matrix3f t_relative_c_to_q = ch_project::prebuilt::homogenized_translate(c * (-1));

	for (auto it = remaining_data.begin(); it < remaining_data.end(); ++it)
	{
		vector2f v_align_p_c = (t_relative_p_to_c * ch_project::conversions::homogenized_position(*it)).operator vector2f();
		vector2f v_align_c_q = (t_relative_c_to_q * ch_project::conversions::homogenized_position(*it)).operator vector2f();

		float p_to_c_det = p_to_c.cross(v_align_p_c);
		float c_to_q_det = c_to_q.cross(v_align_c_q);

		if (p_to_c_det < 0)
			s1.push_back(*it);

		if (c_to_q_det < 0)
			s2.push_back(*it);
	}

	window->add_dots(s1, ch_project::color::red(), 3.0f, 3.0f);
	window->add_dots(s2, ch_project::color::blue(), 3.0f, 3.0f);
	SDL_Delay(quckhull_delay);
	window->clear_dots();

	find_hull_simulation(s1, from, c, hull, window, helpline_color, std::get<0>(r));
	find_hull_simulation(s2, c, to, hull, window, helpline_color, std::get<0>(l));
}

void quickhull_simulation(std::vector<vector2f> considered_points,
	std::shared_ptr<quickhull_window> window)
{
	std::vector<std::tuple<int, vector2f, vector2f>> hull;
	std::sort(
		considered_points.begin(),
		considered_points.end(),
		[](const vector2f& left, const vector2f& right) {return left.X() < right.X(); }
	);

	const int middle_id = g_id;
	std::pair<vector2f, vector2f> maxima = std::make_pair(*considered_points.begin(), *considered_points.rbegin());
	hull.push_back(std::tuple<int, vector2f, vector2f>(middle_id, maxima.first, maxima.second));

	considered_points.erase(considered_points.begin());
	considered_points.pop_back();

	SDL_Delay(quckhull_delay);
	window->set_hull(hull);

	vector2f separating_line = maxima.second - maxima.first;

	std::vector<vector2f> left;
	std::vector<vector2f> right;

	ch_project::matrix3f t_relative_to_separation_line = ch_project::prebuilt::homogenized_translate(maxima.first * (-1));

	for (auto it = considered_points.begin(); it < considered_points.end(); ++it)
	{
		ch_project::vector3f relative_to_separation_line =
			t_relative_to_separation_line * ch_project::conversions::homogenized_position(*it);

		float det = separating_line.cross(relative_to_separation_line.operator vector2f());

		if (det < 0)
			right.push_back(*it);
		else
			left.push_back(*it);
	}

	SDL_Delay(quckhull_delay);

	window->add_dots(right, ch_project::color_rgba(255, 151, 0, 255), 3.0f, 3.0f);
	window->add_dots(left, ch_project::color_rgba(0, 114, 255, 255), 3.0f, 3.0f);

	SDL_Delay(quckhull_delay);

	find_hull_simulation(right, maxima.first, maxima.second, hull, window, ch_project::color_rgba(255, 0, 0, 255), -1);
	find_hull_simulation(left, maxima.second, maxima.first, hull, window, ch_project::color_rgba(0, 0, 255, 255), -1);

	if (hull.size() > 3)
		remove_from_hull(hull, middle_id);

	window->set_hull(hull);
}

void run_quickhull_simulation(std::vector<vector2f> point_cloud)
{
	std::shared_ptr<quickhull_window> q_window(new quickhull_window("Quickhull", point_cloud));

	std::thread t1(quickhull_simulation, point_cloud, q_window);
	q_window->display();

	t1.join();
}

// quickhull performance
void find_hull_performance(const std::vector<vector2f>& remaining_data, const vector2f& from, const vector2f& to,
	std::vector<std::tuple<int, vector2f, vector2f>>& hull, const int& remove)
{
	if (remaining_data.empty())
		return;

	/*Dreiecke für die GUI*/
	std::vector<std::pair<vector2f, vector2f>> helplines;

	vector2f separation_line = to - from;
	vector2f separation_line_normalized = separation_line.normalized();
	ch_project::matrix3f t_relative_to_separation_line = ch_project::prebuilt::homogenized_translate(from * (-1));

	std::pair<float, std::size_t> hull_candidate = std::make_pair(0, 0);

	for (auto it = remaining_data.begin(); it < remaining_data.end(); ++it)
	{
		vector2f v_hyp = (t_relative_to_separation_line * ch_project::conversions::homogenized_position(*it)).operator vector2f();

		vector2f hyp_normalized = v_hyp.normalized();
		float hyp = v_hyp.norm();
		float cosine = cos(hyp_normalized.angle_r(separation_line_normalized));
		float adjacent = cosine * hyp;

		vector2f normal(separation_line_normalized.Y(), separation_line_normalized.X() * (-1));
		float opposite = sqrt(pow(hyp, 2) - pow(adjacent, 2));

		vector2f hl_from = from + (separation_line_normalized * adjacent);
		vector2f hl_to = hl_from + (normal * opposite);

		helplines.push_back(std::make_pair(hl_from, hl_to));

		if (hull_candidate.first < opposite)
			hull_candidate = std::make_pair(opposite, it - remaining_data.begin());
	}

	vector2f c = remaining_data[hull_candidate.second];

	if (remove > -1)
		remove_from_hull(hull, remove);

	std::tuple<int, vector2f, vector2f> r(++g_id, from, c);
	hull.push_back(r);

	std::tuple<int, vector2f, vector2f> l(++g_id, c, to);
	hull.push_back(l);

	vector2f p_to_c = c - from;
	std::vector<vector2f> s1;

	vector2f c_to_q = to - c;
	std::vector<vector2f> s2;

	ch_project::matrix3f t_relative_p_to_c = ch_project::prebuilt::homogenized_translate(from * (-1));
	ch_project::matrix3f t_relative_c_to_q = ch_project::prebuilt::homogenized_translate(c * (-1));

	for (auto it = remaining_data.begin(); it < remaining_data.end(); ++it)
	{
		vector2f v_align_p_c = (t_relative_p_to_c * ch_project::conversions::homogenized_position(*it)).operator vector2f();
		vector2f v_align_c_q = (t_relative_c_to_q * ch_project::conversions::homogenized_position(*it)).operator vector2f();

		float p_to_c_det = p_to_c.cross(v_align_p_c);
		float c_to_q_det = c_to_q.cross(v_align_c_q);

		if (p_to_c_det < 0)
			s1.push_back(*it);

		if (c_to_q_det < 0)
			s2.push_back(*it);
	}

	find_hull_performance(s1, from, c, hull, std::get<0>(r));
	find_hull_performance(s2, c, to, hull, std::get<0>(l));
}

void quickhull_performance(std::vector<vector2f> considered_points)
{
	auto start = std::chrono::steady_clock::now();

	std::vector<std::tuple<int, vector2f, vector2f>> hull;
	std::sort(
		considered_points.begin(),
		considered_points.end(),
		[](const vector2f& left, const vector2f& right) {return left.X() < right.X(); }
	);

	const int middle_id = g_id;
	std::pair<vector2f, vector2f> maxima = std::make_pair(*considered_points.begin(), *considered_points.rbegin());
	hull.push_back(std::tuple<int, vector2f, vector2f>(middle_id, maxima.first, maxima.second));

	considered_points.erase(considered_points.begin());
	considered_points.pop_back();

	vector2f separating_line = maxima.second - maxima.first;

	std::vector<vector2f> left;
	std::vector<vector2f> right;

	ch_project::matrix3f t_relative_to_separation_line = ch_project::prebuilt::homogenized_translate(maxima.first * (-1));

	for (auto it = considered_points.begin(); it < considered_points.end(); ++it)
	{
		ch_project::vector3f relative_to_separation_line =
			t_relative_to_separation_line * ch_project::conversions::homogenized_position(*it);

		float det = separating_line.cross(relative_to_separation_line.operator vector2f());

		if (det < 0)
			right.push_back(*it);
		else
			left.push_back(*it);
	}

	find_hull_performance(right, maxima.first, maxima.second, hull, -1);
	find_hull_performance(left, maxima.second, maxima.first, hull, -1);

	if(hull.size() > 3)
		remove_from_hull(hull, middle_id);

	auto end = std::chrono::steady_clock::now();

	std::cout << "\n\nConvex Hull:" << std::endl;
	int x = 0;
	for (const auto& i : hull)
		std::cout <<"Line :"<< ++x << " (" << std::get<2>(i).X() << "," << std::get<2>(i).Y() << ")" << std::endl;

	// performance
	std::cout << "\nHull found with Quickhull in: " << std::fixed << std::setprecision(3)
		<< std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count() << " s"
		<< " or " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms"
		<< " and " << hull.size() << " Points" << std::endl;

}

#pragma endregion

#pragma region gift wrapping

int giftwrapping_delay = 25;
int orientation(vector2f a, vector2f b, vector2f c)
{
	// crossproduct
	int cross = (b.Y() - a.Y()) * (c.X() - b.X()) - (b.X() - a.X()) * (c.Y() - b.Y());

	// return colinear, clock or counterclock wise
	if (cross == 0)
		return 0;
	return (cross > 0) ? 1 : 2;
}

// giftwrapping simulation
void giftwrapping_simulation(std::vector<vector2f> considered_points, std::shared_ptr<quickhull_window> window)
{
	int leftMostPoint = 0;
	int currentPoint = 0;
	int nextPoint = 0;
	// stores points that form hull
	std::vector<vector2f> hull;

	std::vector<vector2f> left;
	std::vector<std::pair<vector2f, vector2f>> current_to_next;
	std::vector<std::pair<vector2f, vector2f>> current_to_index;
	std::vector<std::pair<vector2f, vector2f>> hull_lines;


	//finds left most point
	for (int i = 1; i < considered_points.size(); i++)
	{
		if (considered_points[i].X() < considered_points[leftMostPoint].X())
			leftMostPoint = i;
	}

	left.push_back(considered_points[leftMostPoint]);
	window->add_dot(considered_points[leftMostPoint], ch_project::color_rgba(255, 165, 0, 255), 15.0f, 15.0f);
	currentPoint = leftMostPoint;

	do
	{
		// add first left most point to the hull
		// then assigns every loop the next estimated point for the hull
		hull.push_back(considered_points[currentPoint]);

		// sets next point in range
		nextPoint = (currentPoint + 1) % considered_points.size();


		// finds the next outer clockwise point
		for (int i = 0; i < considered_points.size(); i++)
		{	
			window->add_lines(hull_lines, ch_project::color_rgba(255, 0, 0, 255), 1.0f, 1.0f);
			current_to_next.push_back(std::make_pair(considered_points[currentPoint], considered_points[nextPoint]));
			window->add_lines(current_to_next, ch_project::color_rgba(0, 0, 255, 255), 1.0f, 1.0f);
			current_to_index.push_back(std::make_pair(considered_points[currentPoint], considered_points[i]));
			window->add_lines(current_to_index, ch_project::color_rgba(60, 179, 113, 255), 1.0f, 1.0f);
			SDL_Delay(giftwrapping_delay);
			current_to_next.clear();
			current_to_index.clear();
			window->clear_lines();

			if (orientation(considered_points[currentPoint], considered_points[i], considered_points[nextPoint]) == 2)
				nextPoint = i;
		}

		hull_lines.push_back(std::make_pair(considered_points[currentPoint], considered_points[nextPoint]));

		// new predicted outer point is the current point
		// gets checked in while until it reaches start point (left most point)
		currentPoint = nextPoint;

	} while (currentPoint != leftMostPoint);

	window->add_lines(hull_lines, ch_project::color_rgba(255, 0, 0, 255), 1.0f, 1.0f);
}

void run_giftwrapping_simulation(std::vector<vector2f> point_cloud)
{
	std::shared_ptr<quickhull_window> q_window(new quickhull_window("run_giftwrapping", point_cloud));

	std::thread t1(giftwrapping_simulation, point_cloud, q_window);
	q_window->display();

	t1.join();
}

//gift wrapping performance
void giftwrapping_performance(std::vector<vector2f> considered_points)
{
	int leftMostPoint = 0;
	int currentPoint = 0;
	int nextPoint = 0;

	// stores points that form hull
	std::vector<vector2f> hull;

	auto start = std::chrono::steady_clock::now();

	//finds left most point
	for (int i = 1; i < considered_points.size(); i++)
	{
		if (considered_points[i].X() < considered_points[leftMostPoint].X())
			leftMostPoint = i;
	}
	currentPoint = leftMostPoint;
	do
	{
		// add first left most point to the hull
		// then assigns every loop the next estimated point for the hull
		hull.push_back(considered_points[currentPoint]);

		// sets next point in range
		nextPoint = (currentPoint + 1) % considered_points.size();

		// finds the next outer clockwise point
		for (int i = 0; i < considered_points.size(); i++)
		{
			if (orientation(considered_points[currentPoint], considered_points[i], considered_points[nextPoint]) == 2)
				nextPoint = i;
		}

		// new predicted outer point is the current point
		// gets checked in while until it reaches start point (left most point)
		currentPoint = nextPoint;

	} while (currentPoint != leftMostPoint);

	auto end = std::chrono::steady_clock::now();

	// print hull
	std::cout << "\nConvex hull:" << std::endl;
	int x = 0;
	for (vector2f i : hull)
		std::cout << "Line: " << ++x << " (" << i.X() << "," << i.Y() << ")" << std::endl;

	// performance
	std::cout << "\nHull found with Gift wrapping in: " << std::fixed << std::setprecision(3)
		<< std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count() << " s"
		<< " or " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms"
		<< " and " << hull.size() << " Points" << std::endl;
}

#pragma endregion

int main(int argc, char* argv[])
{	
	std::vector<vector2f> point_cloud;
	bool close_app = false;
	std::string input;
	int mode = 0; // 0 = performance mode | 1 = visual mode

	while (close_app == false)
	{
		// choose mode
		std::cout << "\n[ 1 ]: Performance mode" << std::endl;
		std::cout << "[ 2 ]: Visual mode" << std::endl;
		std::cout << "[ q ]: Quit" << std::endl;
		std::cout << "\nChoose mode: ";
		std::cin >> input;

		if (input == "1" || input == "2")
		{
			mode = (input == "1") ? 0 : 1;
		}
		else if (input == "q")
		{
			break;
		}
		else
		{
			std::cout << input << " is no valid input" << std::endl;
			continue;
		}

		// choose input data
		std::cout << "\n[ 1 ]: Random generated  " << std::endl;
		std::cout << "[ 2 ]: File import" << std::endl;
		std::cout << "[ 3 ]: Draw points" << std::endl;
		std::cout << "[ q ]: Quit" << std::endl;
		std::cout << "\nChoose how to generate input data: ";
		std::cin >> input;

		if (input == "1")
		{
			std::cout << "\n--> Random generated " << std::endl;
			point_cloud = generate_random_points();
			//save_points_to_file(point_cloud);

			if (point_cloud.size() < 3)
			{
				std::cout << " number of points must be min 3" << std::endl;
				continue;
			}
		}
		else if (input == "2")
		{
			point_cloud = import_file();

			if (point_cloud.size() < 3)
			{
				std::cout << " number of points must be min 3" << std::endl;
				continue;
			}
		}
		else if (input == "3")
		{
			point_cloud = draw_points();
			//save_points_to_file(point_cloud);

			if (point_cloud.size() < 3)
			{
				std::cout << " number of points must be min 3" << std::endl;
				continue;
			}
		}
		else if (input == "q")
		{
			break;
		}
		else
		{
			std::cout << input << " is no valid input" << std::endl;
			continue;
		}

		//start performance or visual mode
		switch (mode)
		{
		case 0:
		{
			giftwrapping_performance(point_cloud);
			quickhull_performance(point_cloud);
			break;
		}
		case 1:
		{
			run_giftwrapping_simulation(point_cloud);
			run_quickhull_simulation(point_cloud);
			break;
		}
		}
	}
	
	return EXIT_SUCCESS;
}
