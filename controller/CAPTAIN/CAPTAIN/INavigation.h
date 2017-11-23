#pragma once
#include "Task.h"
#include "NavigationData.h"
#include "TargetPosition.h"
#include "CoverageRectangle.h"

class INavigation
{
public:
	virtual ~INavigation() {}
	/**
	* \brief   PerformTask overload 1, starts or stops
	* \param task The task to perform, in this case either Start or Stop
	*/
	virtual void PerformTask(const Task task) = 0;
	/**
	* \brief   PerformTask overload 2, calculates a path vector and stores in path_
	* \param task The task to perform, in this case CalcP2P
	* \param target_position The target for the point to point path calculation
	*/
	virtual void PerformTask(const Task task, const TargetPosition target_position) = 0;
	/**
	* \brief   PerformTask overload 3, calculates a path vector and stores in path_
	* \param task The task to perform, in this case CalcCoverageRectangle
	* \param coverage_rectangle The start and end coordinates for the coverage rectangle
	*/
	virtual void PerformTask(const Task task, const CoverageRectangle coverage_rectangle) = 0;
	/**
	* \brief   GetNavData, retrieves the navigation data; path_, completed_path_, pose_, and
	* \return The completed path vector, path vector, ETE (Estimated Time Enroute), and progress in percentage
	*/
	virtual NavigationData GetNavData() = 0;
	/**
	* \brief   SetParameters sets the values of the parameters used in the navigation
	* \param tool_width The toolwidth of the depth sensor that could be connected to the boat
	*/
	virtual void SetParameters(const double tool_width) = 0;
};

