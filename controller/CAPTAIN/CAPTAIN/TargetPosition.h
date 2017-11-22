#pragma once
#include "Coordinate.h"

/**
 * \brief A data class containg the nessesary data to describe a positon to go to.
 */
class TargetPosition
{
public:

	/**
	 * \brief Default constructor
	 */
	TargetPosition(); //Only used to signify an empty object, used for errors
	/**
	 * \brief Explicit constructor
	 * \param coordinate The target positon
	 */
	TargetPosition(const Coordinate coordinate);
	/**
	 * \brief Destructor
	 */
	~TargetPosition();
	/**
	 * \brief Get the target position Coordinate
	 * \return Returns the coordinate of the target positon
	 */
	Coordinate get_target_postion() const;

private:
	/**
	 * \brief the Coordinate describing the target position
	 */
	Coordinate target_position_;
};

