#pragma once
#include <chrono>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <boost/filesystem/operations.hpp>
#include <GeographicLib/Constants.hpp>
#include <GeographicLib/Rhumb.hpp>
#include "Navigation.h"

#define PI 3.141592653589793
#define DISTANCE_BETWEEN_POINTS 10

Navigation::Navigation(IGPS& gps, ITransmitter& transmitter, IAutopilot& auto_pilot) :
	gps_(gps), transmitter_(transmitter), auto_pilot_(auto_pilot),
	completed_path_({ Coordinate(-1, -1), Coordinate(-1, -1) }),
	path_({ Coordinate(-1, -1), Coordinate(-1, -1) }),
	path_start_time_(1), timestamp_(1)
{
	equitorial_radius = static_cast<double>(GeographicLib::Constants::WGS84_a());
	ellipsoid_flattening = static_cast<double>(GeographicLib::Constants::WGS84_f());
}

Navigation::~Navigation()
{

}

//Called by JSONReceiver.ReceiveToNav if func_ is not CalcP2P nor CalcCoverageRectangle
void Navigation::PerformTask(const Task task)
{
	switch (task)
	{
	case 2: //Start

		//Check if a new path is ready to be traversed, otherwise an existing one should be continued
		if (path_start_time_ == 1)
		{
			//Start traversing the path
			start();
			//Update timestamp_ immediately after starting
			setTimestamp();
			//Set path_start_time_
			path_start_time_ = timestamp_;
		}
		else
		{
			//Continue traversing the path
			continuePath();
			//Update timestamp_
			setTimestamp();
			//Check if the path has been fully traversed
			if (completed_path_.size() == path_.size())
			{
				//The path has been traversed, call stop function
				stop();
				//Set start time to 1, so the start function can be called again above
				path_start_time_ = 1;
			}
		}

	case 3: //Stop
		//Call stop function
		stop();
		//Update timestamp_
		setTimestamp();
		break;

	default:
		break;
	}

	//Path to fromNav file
	boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append("JSON\\fromNav\\");
	//Call JSONTransmitter to update fromNav file
	transmitter_.TransmitFromNav(filepath.string(), std::to_string(timestamp_));
}

//Called by JSONReceiver.ReceiveToNav if func_ = CalcP2P
void Navigation::PerformTask(const Task task, const TargetPosition target_position)
{
	if (task == CalcCoverageRectangle)
	{
		//Call calculateP2PPath with the received Targetposition
		calculateP2PPath(target_position);

		//Update timestamp_
		setTimestamp();

		//Path to fromNav file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append("JSON\\fromNav\\");

		//Call JSONTransmitter to update fromNav file
		transmitter_.TransmitFromNav(filepath.string(), std::to_string(timestamp_));
	}
}

//Called by JSONReceiver.ReceiveToNav if func_ = CalcCoverageRectangle
void Navigation::PerformTask(const Task task, const CoverageRectangle coverage_rectangle)
{
	if (task == CalcCoverageRectangle)
	{
		//Call calculateP2PPath with the received Targetposition
		calculateCoverageRectanglePath(coverage_rectangle);

		//Update timestamp_
		setTimestamp();

		//Path to fromNav file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append("JSON\\fromNav\\");

		//Call JSONTransmitter to update fromNav file
		transmitter_.TransmitFromNav(filepath.string(), std::to_string(timestamp_));
	}
}

//Called by JSONTransmitter when requesting data from navigation
NavigationData Navigation::GetNavData()
{
	//Call CalculateETE() to get ETE
	const int ete = calculateEte();

	//Calculate progress by taking the fraction of completed_path.size() and path.size()
	const double progress = static_cast<double>(completed_path_.size()) / static_cast<double>(path_.size());

	return NavigationData(completed_path_, path_, ete, progress);
}

//This function is called by JSONReceiver.ReceiveToNav(), which outputs a float	tool_width
void Navigation::SetParameters(const float tool_width_in)
{
	parameters_.tool_width = tool_width_in;
}

//Called by PerformTask (the P2P overload version)
std::vector<Coordinate> Navigation::calculateP2PPath(const TargetPosition target_position)
{
	//The current coordinate is extracted from the GPS
	const Coordinate start = gps_.GetPose().Coordinate_;

	//The target coordinate is extracted from the TargetPosition input
	const std::vector<Coordinate> end = { target_position.get_target_postion() };

	//Construct the path_ object as a list of the 
	const std::vector<Coordinate> path_segments = calculatePathSegments(start, end, 0);

	//Translate the segments to points
	const std::vector<Coordinate> path = calculatePointsFromSegments(path_segments, 0);

	//Update path_. This is important for the public GetNavData function,
	// and in case the trip is stopped and then resumed at some point
	path_ = path;

	return path;
}

//Called by PerformTask (the CoverageRectangle overload version)
std::vector<Coordinate> Navigation::calculateCoverageRectanglePath(const CoverageRectangle coverage_rectangle)
{
	//The current coordinate is extracted from the GPS
	const Coordinate current = gps_.GetPose().Coordinate_;

	//The start and end coordinates are extracted from the CoverageRectangle input
	const std::vector<Coordinate> rectangle_positions = { coverage_rectangle.get_start_position(), coverage_rectangle.get_end_position() };

	//Construct the path_ object as a list of the 
	const std::vector<Coordinate> path_segments = calculatePathSegments(current, rectangle_positions, 1);

	//Translate the segments to points
	const std::vector<Coordinate> path = calculatePointsFromSegments(path_segments, 1);

	//Update path_. This is important for the public GetNavData function,
	// and in case the trip is stopped and then resumed at some point
	path_ = path;

	return path;
}

//Called by calculateP2PPath, calculateCoverageRectanglePath, and any future path method
std::vector<Coordinate> Navigation::calculatePathSegments
(const Coordinate current_position, const std::vector<Coordinate> key_method_positions, const int method)
{
	//Create the point to point segment vector, insert current position and end position
	std::vector<Coordinate> point_to_point_segments = std::vector<Coordinate>{ current_position, key_method_positions[0] };

	//Create the rectangle segment vector, insert current position and rectangle start position initially
	std::vector<Coordinate> rectangle_segments = std::vector<Coordinate>{ current_position, key_method_positions[0] };

	switch (method)
	{
	case 0: //Point to Point

			//Return the current position and the end position.
		return point_to_point_segments;

	case 1: //Coverage Rectangle

			//Iterate through, hold latitude constant in lines, returns dummy now
		return rectangle_segments;

		//Room to expand with more cases if desired

	default: //This should never happen
		return std::vector<Coordinate>{ Coordinate{ -1, -1 }};
	}
}

std::vector<Coordinate> Navigation::calculatePointsFromSegments(const std::vector<Coordinate> path_segments, const int method) const
{
	//Create the path vector
	std::vector<Coordinate> path;

	switch (method)
	{
	case 0: //Point to Point
		try {
			GeographicLib::Rhumb rhumb(equitorial_radius, ellipsoid_flattening);
			double s12, azi12;
			rhumb.Inverse(path_segments.front().Latitude_, path_segments.front().Longitude_, path_segments.back().Latitude_, path_segments.back().Longitude_, s12, azi12);
			const GeographicLib::RhumbLine line = rhumb.Line(path_segments.front().Latitude_, path_segments.front().Longitude_, azi12);
			// Nominal distance between points
			const double ds0 = DISTANCE_BETWEEN_POINTS;
			// The number of intervals
			const int num = int(ceil(s12 / ds0)); 									  
			// Use intervals of equal length
			const double ds = s12 / num;
			for (int i = 0; i <= num; ++i)
			{
				double lat, lon;
				line.Position(i * ds, lat, lon);
				path.push_back(Coordinate(lat, lon));
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Caught exception: " << e.what() << "\n";
			return std::vector<Coordinate>{ Coordinate{ -1, -1 }};
		}
		break;
	case 1: //Coverage Rectangle

			//Iterate through, hold latitude constant in lines, returns dummy now

			//Room to expand with more cases if desired
		break;
	default: //This should never happen
		path.push_back(Coordinate(-1, -1));
	}
	return path;
}

//Rhumb-distance in kilometres and bearing in degrees
// 1) Using GeographicLib
// 2) Some help taken from https://www.movable-type.co.uk/scripts/latlong.html
std::tuple<double, double> Navigation::calculateRhumbDistanceAndBearing(const Coordinate coordinate_one, const Coordinate coordinate_two) const
{
	//Define a rhumb using the WGS84 constants
	GeographicLib::Rhumb rhumb(equitorial_radius, ellipsoid_flattening);

	//Take the inverse of the defined rhumb with two coordinates; result distance and bearing are put in the last two arguments
	double dist, bearing;
	rhumb.Inverse(coordinate_one.Latitude_, coordinate_one.Longitude_, coordinate_two.Latitude_, coordinate_two.Longitude_, dist, bearing);

	//Check bearing 
	return std::tuple<double, double>{dist, bearing < 0 ? bearing + 360 : bearing};
}

//Rhumb-bearing in kilometres
// 1) Using GeographicLib
// 2) Some help taken from https://www.movable-type.co.uk/scripts/latlong.html
std::tuple<double, std::string> Navigation::calculateXte(const Coordinate coordinate_one, const Coordinate coordinate_two) const
{
	const Pose current = gps_.GetPose();

	std::vector<Coordinate> path;

	GeographicLib::Rhumb rhumb(equitorial_radius, ellipsoid_flattening);
	GeographicLib::Rhumb rhumb_reverse(equitorial_radius, ellipsoid_flattening);

	double s12, s21, s32, azi12, azi21, azi32, startlat, startlon;

	rhumb.Inverse(current.Coordinate_.Latitude_, current.Coordinate_.Longitude_, coordinate_two.Latitude_, coordinate_two.Longitude_, s32, azi32);
	double dist_xte = s32 + 1; //Making certain that dist_xte starts at a value that is *always* too high, otherwise it won't change

	rhumb_reverse.Inverse(coordinate_two.Latitude_, coordinate_two.Longitude_, coordinate_one.Latitude_, coordinate_one.Longitude_, s21, azi21);
	//Reverse the line, and go back from the start point
	const GeographicLib::RhumbLine reverse_line = rhumb_reverse.Line(coordinate_one.Latitude_, coordinate_one.Longitude_, azi21);

	reverse_line.Position(s32, startlat, startlon);

	//Construct the line to look for XTE
	rhumb.Inverse(startlat, startlon, coordinate_two.Latitude_, coordinate_two.Longitude_, s12, azi12);
	const GeographicLib::RhumbLine line = rhumb.Line(startlat, startlon, azi12);

	// Nominal distance between points
	const double ds0 = DISTANCE_BETWEEN_POINTS / 10;

	// The number of intervals: distance between previous and next path coordinates 
	//PLUS the distance from current to next path on both sides, all divided by ds0
	const int num = int(ceil(s32 * 2 / ds0));


	// Use intervals of equal length
	const double ds = s32 * 2 / num;
	for (int i = 0; i <= num; ++i)
	{
		double lat, lon;
		line.Position(i * ds, lat, lon);
		std::cout << lat << " " << lon << std::endl;
		path.push_back(Coordinate(lat, lon));
	}

	for (std::vector<Coordinate>::iterator it = path.begin(); it != path.end(); ++it)
	{
		//Cleverer to do this directly rather than calling calculateRhumbDistanceAndBearing many times, since calling functions isn't free
		GeographicLib::Rhumb rhumb_xte(equitorial_radius, ellipsoid_flattening);
		double dist, bearing;
		rhumb_xte.Inverse(it->Latitude_, it->Longitude_, current.Coordinate_.Latitude_, current.Coordinate_.Longitude_, dist, bearing);
		if (dist < dist_xte)
		{
			dist_xte = dist;
		}
		std::cout << dist << std::endl;
	}
	//rhumb.Inverse(current.Coordinate_.Latitude_, current.Coordinate_.Longitude_, lat3, lon3, dist_xte, bearing_xte);
	std::string direction_to_steer = (azi32 < current.Orientation_) ? "L" : "R";
	return std::tuple<double, std::string>{dist_xte, direction_to_steer};
}

//Called by continuePath to calculate necessary data for Autopilot telegram
std::tuple<double, std::string, double, int, double, double> Navigation::calculateTelegramData()
{
	const Pose current = gps_.GetPose();

	//Throughout this function, path_.at(completed_path_.size()-1) is used. This means "the coordinate of path_ that has not yet been completed"
	//The reason is that indexes of vectors start at zero, while the size of completed_path_ starts at one

	std::tuple<double, double> distance_and_bearing_orig_dest;
	std::tuple<double, std::string> xte_and_direction_to_steer;

	//If the first point on path has not yet been reached, we have prevent taking the -1'th index of path
	if (completed_path_.size() < 1)
	{
		//These measures don't make sense before first path_ coordinate has been reached, instead we insert curr_pose coordinate
		distance_and_bearing_orig_dest = calculateRhumbDistanceAndBearing(current.Coordinate_, path_.at(completed_path_.size()));
		xte_and_direction_to_steer = calculateXte(current.Coordinate_, path_.at(completed_path_.size()));
	}
	//Normal operation
	else
	{
		//Calculate rhumb bearing from last completed waypoint and next waypoint
		distance_and_bearing_orig_dest = calculateRhumbDistanceAndBearing(path_.at(completed_path_.size() - 1), path_.at(completed_path_.size()));

		//Calculate cross track error and direction to steer
		xte_and_direction_to_steer = calculateXte(path_.at(completed_path_.size() - 1), path_.at(completed_path_.size()));
	}

	//Calculate rhumb bearing between current position and next waypoint
	std::tuple<double, double> distance_and_bearing_curr_dest = calculateRhumbDistanceAndBearing(current.Coordinate_, path_.at(completed_path_.size()));

	//Get the cross track error
	double xte_distance = std::get<0>(xte_and_direction_to_steer);

	//Evaluate the direction to steer, "Is it Left?" --> Yes/No
	std::string xte_relative_direction = std::get<1>(xte_and_direction_to_steer);

	//Get rhumb bearing from last completed waypoint to next waypoint
	double bearing_orig_to_dest = std::get<1>(distance_and_bearing_orig_dest);

	//Get next waypoint #
	int dest = completed_path_.size() + 1;

	//Get rhumb bearing from current position to destination
	double bearing_curr_to_dest = std::get<1>(distance_and_bearing_curr_dest);

	//Get rhumb bearing to steer, same as current position to destination because it's rhumb bearing
	double bearing_steer = bearing_curr_to_dest;

	//Return the calculated values
	return std::tuple<double, std::string, double, int, double, double>
		(xte_distance, xte_relative_direction, bearing_orig_to_dest, dest, bearing_curr_to_dest, bearing_steer);
}

//Called by start and continuePath to append a checksum to the telegram, then return the new telegram and its checksum
std::string Navigation::appendChecksumToTelegram(std::string telegram)
{
	//Checksum declaration
	int chk = 0;

	//Loop through telegram to calculate checksum
	for (int i = 0; i < telegram.length(); i++)
	{
		//XOR checksum with telegram index character
		chk ^= telegram[i];
	}

	//Convert the checksum to a string
	std::stringstream ss;
	ss << std::hex << std::uppercase << static_cast<unsigned int>(chk); //eg. ff -> FF; uppercase required for NMEA 
	const std::string checksum(ss.str());

	//Finish telegram
	telegram = "$" + telegram + "*" + checksum;

	return telegram;
}

//Called by GetNavData to get the Ete_ of the NavigationData object
int Navigation::calculateEte() const
{
	//timestamp_ and path_time_start_ are cast and copied
	const double time_now = static_cast<double>(timestamp_);
	const double path_start_time = static_cast<double>(path_start_time_);

	//The size of path_ and completed_path_ are extracted
	const double completed_path_size = completed_path_.size();
	const double path_size = path_.size();

	//Ete is estimated from the fraction of path points that have been completed between the start time
	//and the last time setTimestamp was called ("now" is still ok, since it's *at most* a few milliseconds ago)

	//Timestamp should not be set here, it's more accurate to do it right after a task has been completed,
	//which is exactly what the PerformTask functions do

	//Distance is another measure that could have been used to estimate the Ete
	return static_cast<int>(time_now / path_start_time / (completed_path_size / path_size) - time_now / path_start_time);
}

//Called by PerformTask as an argument to JSONTransmitter.TransmitFromNav
void Navigation::setTimestamp()
{
	//Call the system_clock.now()'s method "time_since_epoch" (epoch meaning 00:00, 1st of January, 1970)
	//and cast to milliseconds. Then update timestamp_ with this new value
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	timestamp_ = int(ms.count());
}

//Called by PerformTask
//NMEA command structure from http://www.gpsinformation.org/dale/nmea.htm
void Navigation::start()
{
	//See continuePath directly under this function to inspect the NMEA GPAPB command structure

	//Call calculateTelegramData to get the cross track error and direction to steer
	std::tuple<double, std::string, double, int, double, double> data = calculateTelegramData();
	const double xte_distance = std::get<0>(data);
	const std::string xte_relative_direction = std::get<1>(data);
	const double orig_to_dest_bearing = std::get<2>(data);
	const int dest = std::get<3>(data);
	const double curr_to_dest_bearing = std::get<4>(data);
	const double steer_bearing = std::get<5>(data);

	//Construct the telegram without the checksum, true relative_direction is left, false is right
	const std::string telegram_no_chksum = "GPAPB,A,A," + std::to_string(xte_distance) + "," +
		xte_relative_direction + ",K,V,V," + std::to_string(orig_to_dest_bearing) + ",T," +
		std::to_string(dest) + "," + std::to_string(curr_to_dest_bearing) + ",T," +
		std::to_string(steer_bearing) + ",T";

	//Complete the telegram and checksum
	const std::string telegram_with_checksum = appendChecksumToTelegram(telegram_no_chksum);

	//Send telegram and checksum to autopilot
	auto_pilot_.Run(telegram_with_checksum);
}

//Called by PerformTask
//NMEA command structure from http://www.gpsinformation.org/dale/nmea.htm
void Navigation::continuePath()
{
	/*$GPAPB, A, A, 0.10, R, N, V, V, 011, M, DEST, 011, M, 011, M * 3C

	where:
		APB     Autopilot format B
		A       Loran - C blink / SNR warning, general warning
		A       Loran - C cycle warning
		0.10    cross - track error distance
		R       steer Right to correct(or L for Left)
		N       cross - track error units - nautical miles(K for kilometers)
		V       arrival alarm - circle
		V       arrival alarm - perpendicular
		011, M   magnetic bearing, origin to destination
		DEST    destination waypoint ID
		011, M   magnetic bearing, present position to destination
		011, M   magnetic heading to steer(bearings could represent True as 033, T)*/

		//----------------------------------------------------------------------------------\\
		//For CAPTAIN, all bearings are True, NOT magnetic, hence the "T"'s in the telegrams\\
		//----------------------------------------------------------------------------------\\

	//See continuePath directly under this function to inspect the NMEA GPAPB command structure

	//Call calculateTelegramData to get the cross track error and direction to steer
	std::tuple<double, std::string, double, int, double, double> data = calculateTelegramData();
	const double xte_distance = std::get<0>(data);
	const std::string xte_relative_direction = std::get<1>(data);
	const double orig_to_dest_bearing = std::get<2>(data);
	const int dest = std::get<3>(data);
	const double curr_to_dest_bearing = std::get<4>(data);
	const double steer_bearing = std::get<5>(data);

	//Construct the telegram without the checksum, true relative_direction is left, false is right
	const std::string telegram_no_chksum = "GPAPB,A,A," + std::to_string(xte_distance) + "," +
		xte_relative_direction + ",K,V,V," + std::to_string(orig_to_dest_bearing) + ",T," +
		std::to_string(dest) + "," + std::to_string(curr_to_dest_bearing) + ",T," +
		std::to_string(steer_bearing) + ",T";

	//Complete the telegram and checksum
	const std::string telegram_with_checksum = appendChecksumToTelegram(telegram_no_chksum);

	//Send telegram and checksum to autopilot
	auto_pilot_.Run(telegram_with_checksum);
}

//Called by PerformTask
void Navigation::stop() const
{
	//Stop the boat
	auto_pilot_.Stop();
}