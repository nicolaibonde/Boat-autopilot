#pragma once
#include "Pose.h"
#include "GPSStatus.h"
#include <thread>

/**
 * \brief An interface that describes how to interact with a GPS reciever
 */
class IGPS
{
public:
	/**
	 * \brief destructor
	 */
	virtual ~IGPS() {};
	/**
	 * \brief Setup the gps and create a thread that runs a function which get the GPS reciever
	 * \return A thread running a function that gets on the GPS reciever
	 */
	virtual std::thread Setup() = 0;
	/**
	 * \brief Gets the latest Pose object.
	 * \return Returns the latest Pose from the GPS reciever.
	 */
	virtual Pose GetPose() = 0;
	/**
	 * \brief Gets the latest speed
	 * \return Returns the latest speed from the GPS reviever.
	 */
	virtual double GetSpeed() = 0;
	/**
	 * \brief Gets the gps status object, with the latest data from the reciver
	 * \return Returns a GPS status object.
	 */
	virtual GPSStatus GetStatus() = 0;
	/**
	 * \brief A gracefull stop for the thread created in Setup
	 * \details A gracefull stop is nice if the thread from Setup is wanted to be run detached.
	 * \see Setup
	 */
	virtual void StopThread() = 0;
};


