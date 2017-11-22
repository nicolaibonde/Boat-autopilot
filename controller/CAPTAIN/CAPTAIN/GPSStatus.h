#pragma once
#include "IStatus.h"
#include "Pose.h"

/**
 * \brief A Status object for a GPS reciever, it implements the IStatus interface
 */
class GPSStatus :
	public IStatus
{
public:
	/**
	 * \brief explicit constructor
	 * \param fix describes how good the precision can become
	 * \param satellites How many satellites are currently connected to the GPS
	 * \param hdop Describes the horizontal dilution of precission
	 * \param fix_timestamp The timestamp of when the fix was last gotten, in UTC
	 * \param pose Where is the gps at; latitude, longitude, orientation
	 */
	GPSStatus(const double fix, const int satellites, const double hdop, const int fix_timestamp, const Pose pose);
	/**
	 * \brief destructor
	 */
	~GPSStatus();
	/**
	 * \brief Get status object as a serialized JSON string
	 * \return Returns a JSON object serialized as a string
	 */
	std::string GetString() override;

private:
	/**
	 * \brief Describes how good the connection is
	 * \details fix can be any of these: \n
	 * 0 = invalid \n
	 * 1 = GPS fix (SPS) \n
	 * 2 = DGPS fix \n
	 * 3 = PPS fix \n
	 * 4 = Real Time Kinematic \n
	 * 5 = Float RTK \n
	 * 6 = estimated (dead reckoning) (2.3 feature) \n
	 * 7 = Manual input mode \n
	 * 8 = Simulation mode \n
	 * \n
	 * More information can be found in the NMEA GGA telegram http://www.gpsinformation.org/dale/nmea.htm#GGA
	 */
	double fix_;
	/**
	 * \brief How many satellites are currently connected to the GPS reciever
	 */
	int satellites_;
	/**
	 * \brief The Horizontal dilution of precision of the GPS reciever
	 */
	double hdop_;

	/**
	 * \brief At what time was there last fix
	 */
	int fix_timestamp_;
	/**
	 * \brief The current position and orientation of the GPS
	 */
	Pose pose_;
};

