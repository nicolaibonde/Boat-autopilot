#pragma once
#include "INavigation.h"
#include "IGPS.h"
#include "IAutopilot.h"
#include "TargetPosition.h"
#include "CoverageRectangle.h"
#include "NavigationData.h"

class ITransmitter;

/**
* \brief   An implementation of the INavigation interface.
* \details This is an implementation of the INavigation interface. \n
* This module is responsible for receiving data from the GPS, calculating paths, ete's, xte, bearings, setting system parameters, \n
* calling TransmitFromNav in the Transmitter, and calling Run and Stop in the Autopilot.\n
* Because Navigation and JSONTransmitter must call function in each other, JSONTransmitter.h has a forward declaration of INavigation,\n
* Navigation.h has a forward declaration of ITransmitter.h, JSONTransmitter.cpp includes INavigation.h, Navigation.cpp includes\n
* ITransmitter.h. The Navigation calls NavAcquisition in JSONTransmitter with a this-pointer parameter, and JSONTransmitter then\n
* saves this object pointer. Through this method, the two objects hold a reference and pointer to each other, respectively.\n
* \see JSONTransmitter.h
*/
class Navigation :
	public INavigation
{
public:
	/**
	* \brief   Constructor
	* \param gps This is a reference to the GPS
	* \param transmitter This is a reference to the Transmitter
	* \param autopilot This is a reference to the Autopilot
	*/
	Navigation(IGPS& gps, ITransmitter& transmitter, IAutopilot& autopilot);
	/**
	 * \brief Destructor
	 */
	~Navigation();
	/**
	* \brief   PerformTask overload 1, starts or stops
	* \param task The task to perform, in this case either Start or Stop
	*/
	void PerformTask(const Task task) override;
	/**
	* \brief   PerformTask overload 2, calculates a path vector and stores in path_
	* \param task The task to perform, in this case CalcP2P
	* \param target_position The target for the point to point path calculation
	*/
	void PerformTask(const Task task, const TargetPosition target_position) override;
	/**
	* \brief   PerformTask overload 3, calculates a path vector and stores in path_
	* \param task The task to perform, in this case CalcCoverageRectangle
	* \param coverage_rectangle The start and end coordinates for the coverage rectangle
	*/
	void PerformTask(const Task task, const CoverageRectangle coverage_rectangle) override;
	/**
	* \brief   GetNavData, retrieves the navigation data; path_, completed_path_, pose_, and 
	* \return The completed path vector, path vector, ETE (Estimated Time Enroute), and progress in percentage
	*/
	NavigationData GetNavData() override;
	/**
	* \brief   SetParameters sets the values of the parameters used in the navigation
	* \param tool_width The toolwidth of the depth sensor that could be connected to the boat
	*/
	void SetParameters(const double tool_width) override;

private:
	//! Reference to the instantiated Ublox_neo7m object
	IGPS& gps_;
	//! Reference to the instantiated Transmitter object
	ITransmitter& transmitter_;
	//! Reference to the instantiated Autopilot object
	IAutopilot& auto_pilot_;
	//! Vector of coordinates describing the completed ordered waypoints in the current path
	std::vector<Coordinate> completed_path_;
	//! Vector of coordinates describing the current path
	std::vector<Coordinate> path_;
	//! The start time for the current path_, used for calculating ete_ and progress_
	unsigned long long path_start_time_;
	//! The current timestamp, updated in each call to PerformTask overload 2
	unsigned long long timestamp_;
	//! The equitorial radius of the earth in the WGS84 model = 6378137.0 m (https://confluence.qps.nl/pages/viewpage.action?pageId=29855173#WorldGeodeticSystem1984(WGS84)-WGS84definitions)
	double equitorial_radius_;
	//! The ellipsoid flattening of the earth in the WGS84 model = 298.257223563 (https://confluence.qps.nl/pages/viewpage.action?pageId=29855173#WorldGeodeticSystem1984(WGS84)-WGS84definitions)
	double ellipsoid_flattening_;

	/**
	* \brief   Struct holding the parameters of the system
	*/
	struct
	{
		//! The toolwidth of the depth sensor that could be connected to the boat
		double tool_width_;
		//! The metric distance between points in straight lines
		double distance_between_points_;
		//! The metric distance between helper points when running the calculateXte algorithm
		double distance_between_xte_points_;
		//! The metric radius of the circle arrival. When the boat's distance from the next path_ Coordinate is lower than this, the next path_ coordinate is pushed to completed_path_
		double waypoint_circle_arrival_;
	} parameters_;

	/**
	 * \brief calculateP2PPath called by PerformTask overload 2, calculates a path between the current point and a TargetPosition
	 * \param targetposition The target of the point to point path
	 * \return The finished path_ vector
	 */
	std::vector<Coordinate> calculateP2PPath(const TargetPosition targetposition);
	/**
	 * \brief calculateCoverageRectanglePath called by PerformTask overload 3, calculates a path from the current position to the start of the CoverageRectangle, and covers the rectangle to the end
	 * \param coverage_rectangle The coverage rectangle to cover
	 * \return The finished path_ vector
	 */
	std::vector<Coordinate> calculateCoverageRectanglePath(const CoverageRectangle coverage_rectangle);
	/**
	 * \brief calculatePathSegments called by calculateP2PPath and calculateCoverageRectanglePath. Calculates the segments of the path_
	 * \param current_position The current position of the boat
	 * \param key_method_positions The key coordinates for the type of calculation being done. End point for target position, and start/end for coverage rectangle
	 * \param method Which method is being used to calculate the segments; point to point or coverage rectangle
	 * \return The vector of coordinates to use in calculatePointsFromSegments
	 */
	std::vector<Coordinate> calculatePathSegments(const Coordinate current_position,
	                                              const std::vector<Coordinate> key_method_positions,
	                                              const int method) const;
	/**
	 * \brief calculatePathSegments called by calculateP2PPath and calculateCoverageRectanglePath. Calculates the path_ from the segments
	 * \param path_segments The output of calculatePathSegments; segments that describe the path and now have to be connected
	 * \param method Which method is being used to calculate the waypoints; point to point or coverage rectangle
	 * \return The finished path_ vector
	 */
	std::vector<Coordinate> calculatePointsFromSegments(std::vector<Coordinate> path_segments, const int method) const;
	/**
	 * \brief calculateTelegram finds and calculates the necessary parameters to build the telegrams that are transmitted to the Autopilot
	 * \return xte_distance, direction_to_steer, orig_to_dest_bearing, dest, curr_to_dest_relative_bearing, steer_bearing
	 * \return The XTE distance, direction to steer, bearing from origin to destination, id of the next waypoint, relative bearing from current position to next waypoint, and bearing to steer
	 */
	std::tuple<double, std::string, double, int, double, double> calculateTelegramData();
	/**
	 * \brief calculateRhumbDistanceAndBearing is used in several functions, and calculates distance and bearing for a rhumb between two coordinates
	 * \param coordinate_one The start coordinate
	 * \param coordinate_two The end coordinate
	 * \return Rhumb distance and bearing
	 */
	std::tuple<double, double> calculateRhumbDistanceAndBearing(const Coordinate coordinate_one,
	                                                            const Coordinate coordinate_two) const;
	/**
	 * \brief calculateXte calculates the projected distance from the boat to a line between the previous waypoint and next waypoint
	 * \param coordinate_one The previous waypoint
	 * \param coordinate_two The next waypoint
	 * \return The XTE (Cross Track Error)
	 */
	double calculateXte(const Coordinate coordinate_one, const Coordinate coordinate_two) const;
	/**
	 * \brief appendCheckToTelegram finishes a telegram to be sent to Autopilot by calculating checksum, and putting a "$" in front and a "*" in the back of the telegram
	 * \param telegram 
	 * \return The finished telegram
	 */
	static std::string appendChecksumToTelegram(std::string telegram);
	/**
	 * \brief calculateEte estimates the time until completion of the path_
	 * \return ETE (Estimated Time Enroute) in milliseconds
	 */
	int calculateEte() const;
	/**
	 * \brief setTimestamp sets the timestamp in PerformTask overload 1 when stop() or start() have been run using the chrono library
	 */
	void setTimestamp();
	/**
	 * \brief start begins traversing the calculated path_, and calls the Autopilot with two telegrams, APB and VTG
	 */
	void start();
	/**
	* \brief stop halts the boat by calling the Autopilot's stop function
	*/
	void stop() const;
};
