#pragma once
#include "IAutopilot.h"
#include "ISpeedMotor.h"
#include "IPositionMotor.h"
#include "MiniPID.h"
#include <boost/algorithm/string.hpp>



/**
 * \brief   Autopilot 
 * \details This is an implementation of the IAutopilot interface. \n
 * It controls the Thruster and the Rudder of a boat, via commands from the Run function. \n
 * It uses 2 PID regulators to set the speed and direction of the boat. \n
 * It's important that the Run function is called with a regular interval, but also not with the same data twice.
 * \see Run()
*/
class Autopilot : public IAutopilot
{
public:

	/**
	 * \brief constructor.
	 * \details Also makes sure that the rudder is centered and the boat is not thrusting
	 * \param rudder a reference to a IPositionMotor object, for controlling direction
	 * \param thruster a refrence to a ISpeedMotor object, for controlling speed
	 */
	Autopilot(IPositionMotor &rudder, ISpeedMotor &thruster);
	
	/**
	 * \brief destructor
	 */
	~Autopilot();

	
	/**
	 * \brief uses a telegram to regulate the speed or the direction.
	 * \details	takes either a NMEA APB http://www.gpsinformation.org/dale/nmea.htm#APB or a NMEA VTG http://www.gpsinformation.org/dale/nmea.htm#VTG \n
	 * it regulates the direction according the APB message and regulates the speed witht the VTG telegram \n
	 * it should be called with a regular interval to avoid PID wind-up.\n
	 * before Run is called the first time SetParameters should have been run once, to set the PID components.
	 * 
	 * \param telegram 
	 * \see SetParamters
	 */
	void Run(std::string telegram) override;
	
	/**
	 * \breif stops the boat
	 * \details This set the speed of the boat to zero and the rudder is centered also set the running_ to false
	 */
	void Stop() override;
	
	/**
	 * \brief Set the PID components for the rudder pid.
	 * \param p proportional component
	 * \param i integral component
	 * \param d derivative component
	 */
	void SetParameters(double p, double i, double d) override;

private:
	/**
	 * \breif Checks wether the given checksum in a telegram is correct
	 * \param telegram This is a string containing a NMEA telegram from start to end
	 * \return wether or not the telegram had a valid checksum
	*/
	bool checksum(std::string telegram);


private:
	//! The proportional component of the autopilot rudder regulator
	double p_;
	//! The integral component of the autopilot rudder regulator
	double i_;
	//! The derivative component of the autopilot rudder regulator
	double d_;
	//! Wether or not the boat is currently running or started
	bool running_;
	//! The max speed the thruster regulator can reach
	double max_speed_; 
	//! The current speed of the motor right now
	double current_speed;

	//! the PID object handeling the rudder ei. direction 
	MiniPID rudder_pid_;
	//! the PID object handeling the thruster ei. speed 
	MiniPID thruster_pid_; 
	//! a reference to the rudder, used to tell the rudder where to point
	IPositionMotor &rudder_; 
	//! a reference to the thruster to tell how fast to spin
	ISpeedMotor &thruster_;
};

