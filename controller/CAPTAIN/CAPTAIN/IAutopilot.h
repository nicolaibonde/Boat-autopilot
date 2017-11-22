#pragma once
#include <string>

/**
 * \brief An Interface describing what is required to be an autopilot in the CAPTAIN system
 */
class IAutopilot
{
public:
	/**
	 * \brief destructor
	 */
	virtual ~IAutopilot() {};
	/**
	 * \brief Should run the autopilot PID one step.
	 * \param telegram a string containg some sort of message, and is defined by the implementer of the Autopilot
	 */
	virtual void Run(std::string telegram) = 0;
	/**
	 * \brief Should stop what ever the autopilot is driving
	 */
	virtual void Stop() = 0;
	/**
	 * \brief should setup the PID terms of the autopilot PID
	 * \param p proportional component
	 * \param i intergral component
	 * \param d derivative component
	 */
	virtual void SetParameters(double p, double i, double d) = 0;
};