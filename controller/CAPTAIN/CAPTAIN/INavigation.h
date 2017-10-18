#pragma once
#include "Task.h"
#include "TaskData.h"
#include "NavigationData.h"

class INavigation
{
public:
	virtual ~INavigation() {}
	virtual void PerformTask(Task task, TaskData task_data) = 0;
	virtual NavigationData GetNavData() = 0;
	virtual void SetParameters(double tool_width) = 0;
};

