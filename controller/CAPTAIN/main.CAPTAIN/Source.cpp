#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <math.h>
#include <CAPTAIN/Coordinate.h>
#include <GeographicLib/Rhumb.hpp>
#include <GeographicLib/Constants.hpp>
#include <iomanip>
#include "CAPTAIN/Pose.h"
#include <vector>

#define PI 3.141592653589793
#define DISTANCE_BETWEEN_POINTS 10

//Rhumb-distance in kilometres and bearing in degrees
// 1) Using GeographicLib
// 2) Some help taken from https://www.movable-type.co.uk/scripts/latlong.html
std::tuple<double,double> calculateRhumbDistanceAndBearing(const Coordinate coordinate_one, const Coordinate coordinate_two)
{
	//Define a rhumb using the WGS84 constants
	GeographicLib::Rhumb rhumb(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());

	//Take the inverse of the defined rhumb with two coordinates; result distance and bearing are put in the last two arguments
	double dist, bearing;
	rhumb.Inverse(coordinate_one.Latitude_, coordinate_one.Longitude_, coordinate_two.Latitude_, coordinate_two.Longitude_, dist, bearing);

	//Check bearing 
	return std::tuple<double, double>{dist, bearing < 0 ? bearing+360 : bearing};
}

//Rhumb-bearing in kilometres
// 1) Using GeographicLib
// 2) Some help taken from https://www.movable-type.co.uk/scripts/latlong.html
std::tuple<double,std::string> calculateXte(const Coordinate coordinate_one, const Coordinate coordinate_two, const Pose current)
{
	std::vector<Coordinate> path;
	

	GeographicLib::Rhumb rhumb(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());
	GeographicLib::Rhumb rhumb_reverse(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());

	double s12, s21, s32, azi12, azi21, azi32, startlat, startlon;
	
	rhumb.Inverse(current.Coordinate_.Latitude_, current.Coordinate_.Longitude_, coordinate_two.Latitude_, coordinate_two.Longitude_, s32, azi32);
	double dist_xte = s32+1; //Making certain that dist_xte starts at a value that is *always* too high, otherwise it won't change

	rhumb_reverse.Inverse(coordinate_two.Latitude_, coordinate_two.Longitude_, coordinate_one.Latitude_, coordinate_one.Longitude_, s21, azi21);
	//Reverse the line, and go back from the start point
	const GeographicLib::RhumbLine reverse_line = rhumb_reverse.Line(coordinate_one.Latitude_, coordinate_one.Longitude_, azi21);
	std::cout << azi21;
	reverse_line.Position(s32, startlat, startlon);
	std::cout << startlat << " " << startlon << std::endl << std::endl;

	//Construct the line to look for XTE
	rhumb.Inverse(startlat, startlon, coordinate_two.Latitude_, coordinate_two.Longitude_, s12, azi12);
	const GeographicLib::RhumbLine line = rhumb.Line(startlat, startlon, azi12);

	// Nominal distance between points
	const double ds0 = DISTANCE_BETWEEN_POINTS/10;

	// The number of intervals: distance between previous and next path coordinates 
	//PLUS the distance from current to next path on both sides, all divided by ds0
	const int num = int(ceil(s32*2 / ds0)); 
	
	
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
		GeographicLib::Rhumb rhumb_xte(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());
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

//Called by calculateP2PPath, calculateCoverageRectanglePath, and any future path method
std::vector<Coordinate> calculatePathSegments
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

std::vector<Coordinate> calculatePointsFromSegments(const std::vector<Coordinate> path_segments, const int method)
{
	//Create the path vector
	std::vector<Coordinate> path;

	switch (method)
	{
	case 0: //Point to Point
		try {
			GeographicLib::Rhumb rhumb(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());
			double s12, azi12;
			rhumb.Inverse(path_segments.front().Latitude_, path_segments.front().Longitude_, path_segments.back().Latitude_, path_segments.back().Longitude_, s12, azi12);
			const GeographicLib::RhumbLine line = rhumb.Line(path_segments.front().Latitude_, path_segments.front().Longitude_, azi12);
			const double ds0 = DISTANCE_BETWEEN_POINTS; // Nominal distance between points
			const int num = int(ceil(s12 / ds0)); // The number of intervals
			//std::cout << std::fixed << std::setprecision(8);
			//std::cout << s12 << std::endl << std::endl;
			
			// Use intervals of equal length
			const double ds = s12 / num;
			for (int i = 0; i <= num; ++i) 
			{
				double lat, lon;
				line.Position(i * ds, lat, lon);
				//std::cout << i << " " << lat << " " << lon << std::endl;
				path.push_back(Coordinate(lat, lon));
			}
			
			//std::cout << std::endl;
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
		path.push_back(Coordinate(-1,-1));
	}
	return path;
}

//Called by continuePath to calculate necessary data for Autopilot telegram
std::tuple<double, std::string, double, int, double, double> calculateTelegramData(Pose curr_pose, std::vector<Coordinate> path_, std::vector<Coordinate> completed_path_)
{

	//Throughout this function, path_.at(completed_path_.size()-1) is used. This means "the coordinate of path_ that has not yet been completed"
	//The reason is that indexes of vectors start at zero, while the size of completed_path_ starts at one

	std::tuple<double, double> distance_and_bearing_orig_dest;
	std::tuple<double, std::string> xte_and_direction_to_steer;

	//If the first point on path has not yet been reached, we have prevent taking the -1'th index of path
	if (completed_path_.size() < 1)
	{
		//These measures don't make sense before first path_ coordinate has been reached, instead we insert curr_pose coordinate
		distance_and_bearing_orig_dest = calculateRhumbDistanceAndBearing(curr_pose.Coordinate_, path_.at(completed_path_.size()));
		xte_and_direction_to_steer = calculateXte(curr_pose.Coordinate_, path_.at(completed_path_.size()), curr_pose );
	}
	//Normal operation
	else
	{
	//Calculate rhumb bearing from last completed waypoint and next waypoint
	distance_and_bearing_orig_dest = calculateRhumbDistanceAndBearing(path_.at(completed_path_.size() - 1), path_.at(completed_path_.size()));
	
	//Calculate cross track error and direction to steer
	xte_and_direction_to_steer = calculateXte(path_.at(completed_path_.size() - 1), path_.at(completed_path_.size()), curr_pose);
	}

	//Calculate rhumb bearing between current position and next waypoint
	std::tuple<double, double> distance_and_bearing_curr_dest = calculateRhumbDistanceAndBearing(curr_pose.Coordinate_, path_.at(completed_path_.size()));

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
std::tuple<std::string, int> appendChecksumToTelegram(std::string telegram)
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

	//Return checksum as a string in the telegram and an int in the tuple, to decrease likelyhood of a 
	//false positive in the Autopilot checksum evaluation due to systematic bias during transfer
	return std::tuple<std::string, int>(telegram, chk);
}

int main()
{
	//Pose pose = { Coordinate{ 56.1714, 10.1902 }, 40};
	//Coordinate start = Coordinate{ 56.1701, 10.19 };
	//std::vector<Coordinate> end = { { 56.1724, 10.191 } };

	Coordinate start = Coordinate{ 56, 11 };
	std::vector<Coordinate> end = { { 56.2, 11.2 } };

	//Construct the path_ vector as a list of the calculated segments
	const std::vector<Coordinate> path_segments = calculatePathSegments(start, end, 0);

	//Translate the segments to coordinates
	const std::vector<Coordinate> path_ = calculatePointsFromSegments(path_segments, 0);

	//The completed_path_ vector
	std::vector<Coordinate> completed_path_ = { { 56, 11 } };

	Pose pose = { Coordinate{ 56.000, 11.00077924 }, 10 };

	//Call calculateTelegramData to get the xte, direction, and bearings
	std::tuple<double, std::string, double, int, double, double> data = calculateTelegramData(pose, path_, completed_path_);

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
	std::tuple<std::string, int> telegram_with_checksum = appendChecksumToTelegram(telegram_no_chksum);

	//Print
	std::cout << std::get<0>(telegram_with_checksum) << std::endl << std::endl;
}
