#pragma once
#include "Coordinate.h"

/**
 * \brief Is to coordinates that describe a rectangle to cover
 */
class CoverageRectangle 
{
public:
	/**
	 * \brief default constructor
	 * \details the coordinates are here set to be -1,-1 to indicate an invalid coordinate and coverage rectangle.
	 */
	CoverageRectangle();

	/**
	 * \brief explicit contructor
	 * \param start_position the first position in a retangle area that should be covered
	 * \param end_position the last postion in a retangle area that should be covered
	 */
	CoverageRectangle(const Coordinate start_position, const Coordinate end_position );
	/**
	 * \brief destructor
	 */
	~CoverageRectangle();

	/**
	 * \brief get the start postion of this CoverageRectangle
	 * \return returns the start position of the area to cover
	 */
	Coordinate get_start_position() const;

	/**
	 * \brief get the end position of this CoverageRectangle
	 * \return returns the end postion of the area to cover
	 */
	Coordinate get_end_position() const;

private:
	/**
	 * \brief A Coordinate of the start position
	 */
	Coordinate start_position_;
	/**
	 * \brief A Coordinate of the end position
	 */
	Coordinate end_position_;
};

