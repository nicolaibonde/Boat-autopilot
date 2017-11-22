#pragma once
#include "Coordinate.h"

/**
 * \brief A data class describing a position and an orientation
 */
class Pose
{
public:
	/**
	 * \brief Explicit constructor
	 * \param coordinate A postion as Coordinate object
	 * \param orientation An orientation in degrees
	 */
	Pose(const Coordinate coordinate, const double orientation);
	/**
	 * \brief Destructor
	 */
	~Pose();

	/**
	 * \brief The position set in the constructor
	 */
	Coordinate Coordinate_;
	/**
	 * \brief The orientation set in the constructor
	 */
	double Orientation_;
};

