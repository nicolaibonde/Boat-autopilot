#pragma once
#include "Coordinate.h"
#include <vector>

/**
 * \brief A data class containing all data that can be gotten from a navigation object
 */
class NavigationData
{
public:
	/**
	 * \brief Explicit constructor
	 * \param completed_path A list of point from the path that have been reached
	 * \param path The full list of points in the path calculated by the navigation
	 * \param ete In milliseconds, what is time til the last point in the path is reached
	 * \param progress What is the completion percentage.
	 */
	NavigationData(std::vector<Coordinate> completed_path, 
		std::vector<Coordinate> path, int ete, double progress);
	/**
	 * \brief Destructor
	 */
	~NavigationData();

public:
	/**
	 * \brief A list of all the points from Path_ that have been reached.
	 */
	std::vector<Coordinate> Completed_path_;
	/**
	 * \brief The full list of points in the path calculated by the navigation
	 */
	std::vector<Coordinate> Path_;
	/**
	 * \brief The time in milliseconds till the last point is reached
	 */
	int Ete_;
	/**
	 * \brief The completion percentage.
	 */
	double Progress_;
};

