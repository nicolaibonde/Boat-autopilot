#pragma once
#include "Task.h"
#include "NavigationData.h"
#include "TargetPosition.h"
#include "CoverageRectangle.h"

class INavigation
{
public:
	virtual ~INavigation() {}
	virtual void PerformTask(const Task task) = 0;
	virtual void PerformTask(const Task task, const TargetPosition target_position) = 0;
	virtual void PerformTask(const Task task, const CoverageRectangle coverage_rectangle) = 0;
	virtual NavigationData GetNavData() = 0;
	virtual void SetParameters(const double tool_width) = 0;
};

