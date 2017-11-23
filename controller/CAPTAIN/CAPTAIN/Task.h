#pragma once

/** \file Task.h
*	This is an file defining an enum of different Tasks types
*/

/**
* \brief Defines a type of motor
*/
enum Task
{
	CalcP2P,
	CalcCoverageRectangle,
	Start,
	Stop,
	None,
};