#include "CoverageRectangle.h"



CoverageRectangle::CoverageRectangle(const Coordinate start_position, const Coordinate end_position) : 
start_position_(start_position), end_position_(end_position)
{
}

CoverageRectangle::~CoverageRectangle()
{
}

Coordinate CoverageRectangle::get_start_position() const
{
	return start_position_;
}

Coordinate CoverageRectangle::get_end_position() const
{
	return end_position_;
}
