#pragma once
#include <chrono>
#include <string>
#include <sstream>
#include <cmath>
#include <boost/filesystem/operations.hpp>
#include <GeographicLib/Rhumb.hpp>
#include "Navigation.h"
#include "ITransmitter.h"
#include <iostream>

Navigation::Navigation(IGPS& gps, ITransmitter& transmitter, IAutopilot& auto_pilot) :
	gps_(gps), transmitter_(transmitter), auto_pilot_(auto_pilot),
	completed_path_({Coordinate(-1, -1), Coordinate(-1, -1)}),
	path_({Coordinate(-1, -1), Coordinate(-1, -1)}),
	path_start_time_(1), timestamp_(1)
{
	//Add pointer to this object in Transmitter
	transmitter_.NavAcquisition(this);

	//Get the equitorial radius and ellipsoid flattening constants from the WGS84 standard from GeographicLib
	equitorial_radius_ = static_cast<double>(GeographicLib::Constants::WGS84_a());
	ellipsoid_flattening_ = static_cast<double>(GeographicLib::Constants::WGS84_f());

	//tool_width can be changed at any time if the parameter profile has a tool_width member
	parameters_.tool_width_ = 1;

	//These values for distance between points, distance between xte points, and the circle arrival radius
	//are constant for the moment, setters could be made if the parameters were added in receiver and on the website
	parameters_.distance_between_points_ = 10;
	parameters_.distance_between_xte_points_ = 1;
	parameters_.waypoint_circle_arrival_ = 5;
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
			start();
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
		break;

	case 3: //Stop
		//Call stop function
		stop();
		//Update timestamp_
		setTimestamp();
		break;

	default:
		break;
	}

	//Call JSONTransmitter to update fromNav file
	transmitter_.TransmitFromNav(std::to_string(timestamp_));
}

//Called by JSONReceiver.ReceiveToNav if func_ = CalcP2P
void Navigation::PerformTask(const Task task, const TargetPosition target_position)
{
	if (task == CalcP2P)
	{
		//Call calculateP2PPath with the received Targetposition
		calculateP2PPath(target_position);
		
		//Update timestamp_
		setTimestamp();
		
		//Call JSONTransmitter to update fromNav file
		transmitter_.TransmitFromNav(std::to_string(timestamp_));
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

		//Call JSONTransmitter to update fromNav file
		transmitter_.TransmitFromNav(std::to_string(timestamp_));
	}
}

//Called by JSONTransmitter when requesting data from navigation
NavigationData Navigation::GetNavData()
{
	//Call CalculateETE() to get ETE
	const int ete = calculateEte();

	//Calculate progress by taking the fraction of completed_path.size() and path.size()
	const double progress = 100* static_cast<double>(completed_path_.size()) / static_cast<double>(path_.size());

	return NavigationData(completed_path_, path_, ete, progress);
}

//This function is called by JSONReceiver.ReceiveToNav(), which outputs a float	tool_width
void Navigation::SetParameters(const double tool_width_in)
{
	parameters_.tool_width_ = tool_width_in;
}

//Called by PerformTask (the P2P overload version)
std::vector<Coordinate> Navigation::calculateP2PPath(const TargetPosition target_position)
{
	//The path_ and completed_path_ lists are first emptied
	path_ = {};
	completed_path_ = {};

	//The current coordinate is extracted from the GPS
	const Coordinate start = gps_.GetPose().Coordinate_;

	//The target coordinate is extracted from the TargetPosition input
	const std::vector<Coordinate> end = {target_position.get_target_postion()};

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
	//The path_ and completed_path_ lists are first emptied
	path_ = {};
	completed_path_ = {};

	//The current coordinate is extracted from the GPS
	const Coordinate current = gps_.GetPose().Coordinate_;

	//The start and end coordinates are extracted from the CoverageRectangle input
	const std::vector<Coordinate> rectangle_positions = {
		coverage_rectangle.get_start_position(), coverage_rectangle.get_end_position()
	};

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
(const Coordinate current_position, const std::vector<Coordinate> key_method_positions, const int method) const
{
	switch (method)
	{
	case 0: //Point to Point
		{
			//Create the point to point segment vector, insert current position and end position
			std::vector<Coordinate> point_to_point_segments = std::vector<Coordinate>{current_position, key_method_positions[0]};

			//Return the current position and the end position.
			return point_to_point_segments;
		}
	case 1: //Coverage Rectangle
		{
			//Create the rectangle segment vector, insert current position and rectangle start position initially
			std::vector<Coordinate> rectangle_segments = std::vector<Coordinate>{current_position};

			//Helper points for the "other two corners" of the rectangle
			const Coordinate help_point_at_start = {key_method_positions[0].Latitude_ , key_method_positions[1].Longitude_};
			const Coordinate help_point_at_end = {key_method_positions[1].Latitude_ , key_method_positions[0].Longitude_};

			//Helper lines 
			std::vector<Coordinate> help_line_to_end = {{}};
			std::vector<Coordinate> help_line_from_start = {{}};

			GeographicLib::Rhumb rhumb_one(equitorial_radius_, ellipsoid_flattening_);
			GeographicLib::Rhumb rhumb_two(equitorial_radius_, ellipsoid_flattening_);
			GeographicLib::Rhumb rhumb_three(equitorial_radius_, ellipsoid_flattening_);
			double s12_one, azi12_one, s12_two, azi12_two, end_distance, end_azi;

			//Take inverse rhumb lines between first help point and end, and then between start and second help point
			rhumb_one.Inverse(help_point_at_start.Latitude_, help_point_at_start.Longitude_, key_method_positions[1].Latitude_,
			                  key_method_positions[1].Longitude_, s12_one, azi12_one);
			rhumb_two.Inverse(key_method_positions[0].Latitude_, key_method_positions[0].Longitude_, help_point_at_end.Latitude_,
			                  help_point_at_end.Longitude_, s12_two, azi12_two);

			//Make rhumb lines given the azimuths from previous inversions. Begin at first help point for the first line,
			//and from start for the second line, to make sure the end stretch is properly set up
			//This means that any issues with tool_width vs. actual size of rectangle is dealt with at the start of the rectangle
			const GeographicLib::RhumbLine line_one = rhumb_one.Line(help_point_at_start.Latitude_,
			                                                         help_point_at_start.Longitude_, azi12_one);
			const GeographicLib::RhumbLine line_two = rhumb_two.Line(key_method_positions[0].Latitude_,
			                                                         key_method_positions[0].Longitude_, azi12_two);

			const double ds0 = parameters_.tool_width_; // Nominal distance between points on help lines
			const int num = int(floor(s12_one / ds0)); // The number of intervals, same for both help lines

			// Use intervals of equal length
			const double ds = s12_one / num;
			for (int i = 0; i <= num; ++i)
			{
				double lat1, lon1, lat2, lon2;
				line_one.Position(i * ds, lat1, lon1);
				line_two.Position(i * ds, lat2, lon2);
				help_line_to_end.push_back(Coordinate(lat1, lon1));
				help_line_from_start.push_back(Coordinate(lat2, lon2));
			}

			rhumb_three.Inverse(help_line_from_start.back().Latitude_, help_line_from_start.back().Longitude_,
			                    help_point_at_end.Latitude_, help_point_at_end.Longitude_, end_distance, end_azi);

			//The rectangle is "well-behaved"; we can aim for help point at start immediately
			if (num % 2 == 0)
			{
				//Uncomment this line to get useful information about the type of CoverageRectangle that is created
				//std::cout << R"(Path is "well-behaved")" << std::endl << std::endl;

				for (int i = 0; i <= num; ++i)
				{
					if (i % 2 == 0) //Path going from line (from start) to line (to end)
					{
						rectangle_segments.push_back(help_line_from_start[i]);
						rectangle_segments.push_back(help_line_to_end[i]);
					}
					else //Path going from line (to end) to line (from start)
					{
						rectangle_segments.push_back(help_line_to_end[i]);
						rectangle_segments.push_back(help_line_from_start[i]);
					}
				}
			}
			else //The rectangle is not "well-behaved"; we have to do further checks 
			{
				if (parameters_.tool_width_ / 2 > end_distance)
				{
					//Uncomment this line to get useful information about the type of CoverageRectangle that is created
					//std::cout << R"(Path is not "well-behaved", but with sufficient toolwidth)" << std::endl << std::endl;
					//The remaining slice of rectangle can be covered while doing a pass of the second line. 
					//Insert start point, then iterate as before from 1 instead of 0
					rectangle_segments.push_back(help_line_from_start[0]);

					for (int i = 1; i <= num; ++i)
					{
						if (i % 2 == 0) //Path going from line (to end) to line (from start)
						{
							rectangle_segments.push_back(help_line_to_end[i]);
							rectangle_segments.push_back(help_line_from_start[i]);
						}
						else //Path going from line (from start) to line (to end)
						{
							rectangle_segments.push_back(help_line_from_start[i]);
							rectangle_segments.push_back(help_line_to_end[i]);
						}
					}
				}
				else
				{
					//Uncomment this line to get useful information about the type of CoverageRectangle that is created
					//std::cout << R"(Path is not "well-behaved", and without sufficient toolwidth)" << std::endl << std::endl;
					//Worst case scenario
					//The remaining slice of rectangle can NOT be covered while doing a pass of the second line. 
					//We need to add a new line manually
					//Insert start point, new line, and only then iterate as before from 1 instead of 0
					rectangle_segments.push_back(help_line_from_start[0]);
					rectangle_segments.push_back(help_line_to_end[0]);
					rectangle_segments.push_back({
						(help_line_to_end[1].Latitude_ - help_line_to_end[0].Latitude_) / 2, help_line_to_end[0].Longitude_
					});
					rectangle_segments.push_back({
						(help_line_to_end[1].Latitude_ - help_line_to_end[0].Latitude_) / 2, help_line_from_start[0].Longitude_
					});

					for (int i = 1; i <= num; ++i)
					{
						if (i % 2 == 0) //Path going from line (from start) to line (to end)
						{
							rectangle_segments.push_back(help_line_from_start[i]);
							rectangle_segments.push_back(help_line_to_end[i]);
						}
						else //Path going from line (to end) to line (from start)
						{
							rectangle_segments.push_back(help_line_to_end[i]);
							rectangle_segments.push_back(help_line_from_start[i]);
						}
					}
				}
			}

			return rectangle_segments;

			//Room to expand with more cases if desired
		}
	default: //This should never happen
		{
			return std::vector<Coordinate>{Coordinate{-1, -1}};
		}
	}
}

std::vector<Coordinate> Navigation::calculatePointsFromSegments(std::vector<Coordinate> path_segments,
                                                                const int method) const
{
	//Create the path vector
	std::vector<Coordinate> path = { { path_segments.front().Latitude_, path_segments.front().Longitude_ } };

	switch (method)
	{
	case 0: //Point to Point
	{
		GeographicLib::Rhumb rhumb(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());
		double s12, azi12;
		rhumb.Inverse(path_segments.front().Latitude_, path_segments.front().Longitude_, path_segments.back().Latitude_,
			path_segments.back().Longitude_, s12, azi12);
		const GeographicLib::RhumbLine line = rhumb.Line(path_segments.front().Latitude_, path_segments.front().Longitude_,
			azi12);

		// Nominal distance between points
		const double ds0 = parameters_.distance_between_points_;

		// The number of intervals
		const int num = int(ceil(s12 / ds0));

		// Use intervals of equal length
		const double ds = s12 / num;

		for (int i = 1; i <= num; ++i)
		{
			double lat, lon;
			line.Position(i * ds, lat, lon);
			if (path.back().Latitude_ != lat || path.back().Longitude_ != lon) //Check for duplicates
			{
				path.push_back(Coordinate(lat, lon));
			}
		}
		break;
	}
	case 1: //Coverage Rectangle
	{
		//We need to use pi in this case
		double pi = GeographicLib::Math::pi();

		for (std::vector<Coordinate>::iterator it = path_segments.begin(); it != prev(path_segments.end()); ++it)
		{
			GeographicLib::Rhumb rhumb(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());
			double s12, azi12;
			const double lat1 = it->Latitude_;
			const double lon1 = it->Longitude_;
			++it;
			const double lat2 = it->Latitude_;
			const double lon2 = it->Longitude_;
			--it;

			rhumb.Inverse(lat1, lon1, lat2, lon2, s12, azi12);
			const GeographicLib::RhumbLine line = rhumb.Line(lat1, lon1, azi12);

			// Nominal distance between points
			const double ds0 = parameters_.distance_between_points_;

			// The number of intervals
			const int num = int(floor(s12 / ds0));

			// Use intervals of equal length
			const double ds = s12 / num;

			//Check if latitudes are the same, if true this will be a horizontal line across the rectangle
			//The function will also enter this loop at the very start when the boat isn't in the rectangle yet 
			if (lat2 == lat1 || path.size() < 2)
			{
				for (int i = 1; i <= num; ++i)
				{
					double lat, lon;
					line.Position(i * ds, lat, lon);
					if (path.back().Latitude_ != lat || path.back().Longitude_ != lon) //Check for duplicates
					{
						path.push_back(Coordinate(lat, lon));
					}
				}
			}
			else //This is a vertical line; we need to wrap around the outside of the rectangle to adjust our position
			{
				//We need the longitude of the previous point to check which direction we're coming from
				const double lon_old = path.at(path.size() - 2).Longitude_;

				if (path.back().Latitude_ != lat1 || path.back().Longitude_ != lon1) //Check for duplicates
				{
					//Push the end of the old line
					path.push_back(Coordinate(lat1, lon1));
				}

				//We need to check which direction we're going, "up" or "down". Check next latitude.
				const bool going_to_higher_latitude = lat2 > lat1 ? true : false;

				//We need to check which direction we came from, "right" or "left". Check old longitude.
				const bool coming_from_higher_longitude = lon_old > lon1 ? true : false;

				//Make a rhumb to find the diameter of the circle to traverse
				GeographicLib::Rhumb rhumb_diameter(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());
				GeographicLib::Rhumb rhumb_center(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());
				double s_dia, azi_dia;
				rhumb_diameter.Inverse(lat1, lon1, lat2, lon2, s_dia, azi_dia);

				//Calculate radius and center latitude
				const double circle_radius = s_dia / 2;
				const double circle_center_lat = (lat2 + lat1) / 2;

				//Make a line with constant latitude from the center to the periphery
				const GeographicLib::RhumbLine line_center = rhumb_center.Line(circle_center_lat, lon1, coming_from_higher_longitude ? 270 : 90);
				double lat_periphery_outer, lon_periphery_outer;
				line_center.Position(circle_radius, lat_periphery_outer, lon_periphery_outer);

				//Calculate radii with respect to latitude and longitude (These will differ slightly since we're using rhumb)
				const double circle_radius_by_latitude = abs(lat2 - lat1) / 2;
				const double circle_radius_by_longitude = abs(lon_periphery_outer - lon1) / 2;

				// Nominal distance between points
				const double ds_circle = parameters_.distance_between_points_;

				//The number of intervals - we're dealing with a half circle
				int num_circle = int(floor(circle_radius*pi / ds_circle));

				//Minimum 5 intervals
				if (num_circle < 5)
				{
					num_circle = 5;
				}

				//Angular distance between each point
				const double theta = 180 / num_circle;

				for (int i = 0; i <= num_circle; i++)
				{
					//Current angle
					const double current_angle = theta*i;

					//Trigonometric functions to navigate the half circle. cos can go negative, sin cannot (in this case)
					const double a = abs(cos(current_angle*pi / 180)*circle_radius);
					const double b = sin(current_angle*pi / 180)*circle_radius;

					//Modifiers to latitude and longitude. Note that longitude_modifier can also be negative
					const double latitude_modifier = (circle_radius - a) / circle_radius*circle_radius_by_latitude;
					const double longitude_modifier = (1 - (circle_radius - b) / circle_radius)*circle_radius_by_longitude;

					double lat_on_circle, lon_on_circle;

					//Latitude and longitude must be modified in different ways depending on their position in the circle
					if (current_angle < 90) //First half of the circle, add or subtract the modifier to the initial position
					{
						lat_on_circle = going_to_higher_latitude ? lat1 + latitude_modifier : lat1 - latitude_modifier;

						lon_on_circle = coming_from_higher_longitude ? lon1 - longitude_modifier : lon1 + longitude_modifier;
					}
					else //Second half of the circle, add or subtract the modifier from the final position
					{
						lat_on_circle = going_to_higher_latitude ? lat2 - latitude_modifier : lat2 + latitude_modifier;

						lon_on_circle = coming_from_higher_longitude ? lon2 - longitude_modifier : lon2 + longitude_modifier;
					}

					if (path.back().Latitude_ != lat_on_circle || path.back().Longitude_ != lon_on_circle) //Check for duplicates
					{
					path.push_back(Coordinate(lat_on_circle, lon_on_circle));
					}
				}

				//Push the start of the new line
				if (path.back().Latitude_ != lat2 || path.back().Longitude_ != lon1) //Check for duplicates
				{
					path.push_back(Coordinate{ lat2, lon1 });
				}
			}
		}
	}
	break;

	default: //This should never happen
	{
		path.push_back(Coordinate(-1, -1));
		break;
	}
	}
	return path;
}

//Rhumb-distance in kilometres and bearing in degrees
// 1) Using GeographicLib
// 2) Some help taken from https://www.movable-type.co.uk/scripts/latlong.html
std::tuple<double, double> Navigation::calculateRhumbDistanceAndBearing(const Coordinate coordinate_one,
                                                                        const Coordinate coordinate_two) const
{
	//Define a rhumb using the WGS84 constants
	GeographicLib::Rhumb rhumb(equitorial_radius_, ellipsoid_flattening_);

	//Take the inverse of the defined rhumb with two coordinates; result distance and bearing are put in the last two arguments
	double dist, bearing;
	rhumb.Inverse(coordinate_one.Latitude_, coordinate_one.Longitude_, coordinate_two.Latitude_, coordinate_two.Longitude_,
	              dist, bearing);

	//Check bearing 
	return std::tuple<double, double>{dist, bearing < 0 ? bearing + 360 : bearing};
}

//Rhumb-bearing in kilometres
// 1) Using GeographicLib
// 2) Some help taken from https://www.movable-type.co.uk/scripts/latlong.html
double Navigation::calculateXte(const Coordinate coordinate_one, const Coordinate coordinate_two) const
{
	std::vector<Coordinate> path;

	const Pose curr_pose = gps_.GetPose();

	GeographicLib::Rhumb rhumb(equitorial_radius_, ellipsoid_flattening_);
	GeographicLib::Rhumb rhumb_reverse(equitorial_radius_, ellipsoid_flattening_);

	double s12, s21, s32, azi12, azi21, azi32, startlat, startlon;

	rhumb.Inverse(curr_pose.Coordinate_.Latitude_, curr_pose.Coordinate_.Longitude_, coordinate_two.Latitude_,
	              coordinate_two.Longitude_, s32, azi32);
	double dist_xte = s32 + 1;
	//Making certain that dist_xte starts at a value that is *always* too high, otherwise it won't change

	rhumb_reverse.Inverse(coordinate_two.Latitude_, coordinate_two.Longitude_, coordinate_one.Latitude_,
	                      coordinate_one.Longitude_, s21, azi21);

	//Reverse the line, and go back from the start point
	const GeographicLib::RhumbLine reverse_line = rhumb_reverse.Line(coordinate_one.Latitude_, coordinate_one.Longitude_,azi21);
	reverse_line.Position(s32, startlat, startlon);

	//Construct the line to look for XTE
	rhumb.Inverse(startlat, startlon, coordinate_two.Latitude_, coordinate_two.Longitude_, s12, azi12);
	const GeographicLib::RhumbLine line = rhumb.Line(startlat, startlon, azi12);

	// Nominal distance between points
	const double ds0 = parameters_.distance_between_xte_points_;

	// The number of intervals: distance between previous and next path coordinates 
	//PLUS the distance from current to next path on both sides, all divided by ds0
	const int num = int(ceil(s32 * 2 / ds0));

	// Use intervals of equal length
	const double ds = s32 * 2 / num;
	for (int i = 0; i <= num; ++i)
	{
		double lat, lon;
		line.Position(i * ds, lat, lon);
		path.push_back(Coordinate(lat, lon));
	}

	for (std::vector<Coordinate>::iterator it = path.begin(); it != path.end(); ++it)
	{
		//Cleverer to do this directly rather than calling calculateRhumbDistanceAndBearing many times, since calling functions isn't free
		GeographicLib::Rhumb rhumb_xte(equitorial_radius_, ellipsoid_flattening_);
		double dist, bearing;
		rhumb_xte.Inverse(it->Latitude_, it->Longitude_, curr_pose.Coordinate_.Latitude_, curr_pose.Coordinate_.Longitude_,
		                  dist, bearing);
		if (dist < dist_xte)
		{
			dist_xte = dist;
		}
	}

	//The telegram requires xte in kilometres or nautical miles. Convert from meters to kilometers and return
	return dist_xte / 1000;

}

//Called by continuePath to calculate necessary data for Autopilot telegram
std::tuple<double, std::string, double, int, double, double> Navigation::calculateTelegramData()
{
	const Pose curr_pose = gps_.GetPose();

	//Immediately check if completed_path_ must be updated
	//Find distance between next waypoint in path_ and current location
	std::tuple<double, double> next_path_curr_location_distance = calculateRhumbDistanceAndBearing(curr_pose.Coordinate_,
		path_.at(completed_path_.size()));
	
	//If the distance is smaller than the circle arrival constant, then push the current path_ 
	//destination into completed_path_
	if (std::get<0>(next_path_curr_location_distance) < parameters_.waypoint_circle_arrival_)
	{
		completed_path_.push_back(path_.at(completed_path_.size()));
	}

	//Throughout this function, path_.at(completed_path_.size()-1) is used. This means "the coordinate of path_ that has not yet been completed"
	//The reason is that indexes of vectors start at zero, while the size of completed_path_ starts at one

	std::tuple<double, double> orig_dest_distance_and_bearing;
	double xte;

	//If the first point on path has not yet been reached, we have to prevent taking the -1'th index of path
	if (completed_path_.size() < 1)
	{
		//These measures don't make sense before first path_ coordinate has been reached, instead we insert curr_pose coordinate
		orig_dest_distance_and_bearing = calculateRhumbDistanceAndBearing(path_.at(0), path_.at(1));
		xte = calculateXte(path_.at(0), path_.at(1));
	}
	else if (completed_path_.size() == path_.size()) //Path has been completed
	{
		//Return some values that are sensible when the path has been succesfully traversed
		return std::tuple<double, std::string, double, int, double, double>
			(0, "L", 0, 0, 0, 0);
	}
		//Normal operation
	else
	{
		//Calculate rhumb bearing from last completed waypoint and next waypoint
		orig_dest_distance_and_bearing = calculateRhumbDistanceAndBearing(path_.at(completed_path_.size() - 1),
		                                                                  path_.at(completed_path_.size()));

		//Calculate cross track error and direction to steer
		xte = calculateXte(path_.at(completed_path_.size() - 1), path_.at(completed_path_.size()));
	}

	//Calculate rhumb bearing between current position and next waypoint
	std::tuple<double, double> distance_and_bearing_curr_dest = calculateRhumbDistanceAndBearing(
		curr_pose.Coordinate_, path_.at(completed_path_.size()));

	//Get the cross track error
	const double xte_distance = xte;

	//Get absolute rhumb bearing from last completed waypoint to next waypoint
	const double orig_to_dest_bearing = std::get<1>(orig_dest_distance_and_bearing);

	//Get next waypoint #
	const int dest = completed_path_.size() + 1;

	//Get absolute rhumb bearing to steer
	const double steer_bearing = std::get<1>(distance_and_bearing_curr_dest);

	//Get direction to steer and relative rhumb bearing from current position to destination
	std::string direction_to_steer;
	double curr_to_dest_relative_bearing = std::get<1>(distance_and_bearing_curr_dest) - curr_pose.Orientation_;

	if (curr_to_dest_relative_bearing > 0)
		//Check bearing from current position to destination is higher than boat's own orientation
	{
		if (curr_to_dest_relative_bearing < 180) //Check if clockwise (right) is the shortest route
		{
			//The relative bearing is already correct
			direction_to_steer = "R";
		}
		else //Quicker to turn clockwise (left)
		{
			//Subtract the relative bearing from a full circle and turn the other way
			curr_to_dest_relative_bearing = 360 - curr_to_dest_relative_bearing;
			direction_to_steer = "L";
		}
	}
	else //Orientation is higher than bearing from current position to destination
	{
		if (-curr_to_dest_relative_bearing < 180) //Check if clockwise (left) is the shortest route
		{
			//The sign of the relative bearing must be flipped
			curr_to_dest_relative_bearing = -curr_to_dest_relative_bearing;
			direction_to_steer = "L";
		}
		else //Quicker to turn counterclockwise (right)
		{
			//Add the relative bearing (which is negative) to a full circle and turn the other way
			curr_to_dest_relative_bearing = 360 + curr_to_dest_relative_bearing;
			direction_to_steer = "R";
		}
	}

	//Return the calculated values
	return std::tuple<double, std::string, double, int, double, double>
		(xte_distance, direction_to_steer, orig_to_dest_bearing, dest, curr_to_dest_relative_bearing, steer_bearing);
}

//Called by start and continuePath to append a checksum to the telegram, then return the new telegram and its checksum
std::string Navigation::appendChecksumToTelegram(std::string telegram)
{
	//Checksum declaration
	int chk = 0;

	//Loop through telegram to calculate checksum
	for (unsigned int i = 0; i < telegram.length(); i++)
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
	if (completed_path_.size() > 0)
	{
		//timestamp_ and path_time_start_ are cast and copied
		const double delta_t = static_cast<double>(timestamp_ - path_start_time_);

		//The size of path_ and completed_path_ are extracted, set completed_path_ to at least 1
		const double completed_path_size = completed_path_.size() > 0 ? completed_path_.size() : 1;
		const double path_size = path_.size();

		//Ete is estimated from the fraction of path points that have been completed between the start time
		//and the last time setTimestamp was called ("now" is still ok, since it's *at most* a few milliseconds ago)

		//Timestamp should not be set here, it's more accurate to do it right after a task has been completed,
		//which is exactly what the PerformTask functions do

		//Distance is another measure that could have been used to estimate the Ete
		const int ete = static_cast<const int>(delta_t * (path_size / completed_path_size) - delta_t);

		return ete;
	}

	return -1;
}

//Called by PerformTask as an argument to JSONTransmitter.TransmitFromNav
void Navigation::setTimestamp()
{
	//Call the system_clock.now()'s method "time_since_epoch" (epoch meaning 00:00, 1st of January, 1970)
	//and cast to milliseconds. Then update timestamp_ with this new value
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	timestamp_ = static_cast<unsigned long long>(ms.count());
}

//Called by PerformTask
//NMEA command structure from http://www.gpsinformation.org/dale/nmea.htm
void Navigation::start()
{
	/*$GPAPB, A, A, 0.10, R, N, V, V, 011, M, DEST, 011, M, 011, M * 3C

	where:
	APB      Autopilot format B
	A        Loran - C blink / SNR warning, general warning
	A        Loran - C cycle warning
	0.10     cross - track error distance
	R        steer Right to correct(or L for Left)
	N        cross - track error units - nautical miles(K for kilometers)
	V        arrival alarm - circle
	V        arrival alarm - perpendicular
	011, M   magnetic bearing, origin to destination
	DEST     destination waypoint ID
	011, M   magnetic relative bearing, present position to destination
	011, M   magnetic bearing to steer(bearings could represent True as 033, T)*/

	/*$GPVTG, 054.7, T, 034.4, M, 005.5, N, 010.2, K * 48

	where:
	VTG			Track made good and ground speed
	054.7, T    True track made good(degrees)
	034.4, M    Magnetic track made good
	005.5, N    Ground speed, knots
	010.2, K    Ground speed, Kilometers per hour*/

	//----------------------------------------------------------------------------------\\
	//For CAPTAIN, all bearings are True, NOT magnetic, hence the "T"'s in the telegrams\\
	//----------------------------------------------------------------------------------\\

	//Call calculateTelegramData to get the cross track error and direction to steer
	std::tuple<double, std::string, double, int, double, double> data = calculateTelegramData();
	const double xte_distance = std::get<0>(data);
	const std::string xte_relative_direction = std::get<1>(data);
	const double orig_to_dest_bearing = std::get<2>(data);
	const int dest = std::get<3>(data);
	const double curr_to_dest_relative_bearing = std::get<4>(data);
	const double steer_bearing = std::get<5>(data);

	//Construct the telegram without the checksum, true relative_direction is left, false is right
	const std::string apb_telegram_no_chksum = "GPAPB,A,A," + std::to_string(xte_distance) + "," +
		xte_relative_direction + ",K,V,V," + std::to_string(orig_to_dest_bearing) + ",T," +
		std::to_string(dest) + "," + std::to_string(curr_to_dest_relative_bearing) + ",T," +
		std::to_string(steer_bearing) + ",T";

	//Construct the telegram without the checksum, true relative_direction is left, false is right
	const std::string vtg_telegram_no_chksum = "GPVTG,,,,," + std::to_string(gps_.GetSpeed()/1.852) + ",N," +
		std::to_string(gps_.GetSpeed()) + ",K";

	//Complete the telegram and checksum
	const std::string apb_telegram_with_checksum = appendChecksumToTelegram(apb_telegram_no_chksum);

	//Complete the telegram and checksum
	const std::string vtg_telegram_with_checksum = appendChecksumToTelegram(vtg_telegram_no_chksum);


	auto_pilot_.Run(vtg_telegram_with_checksum);
	//Send APB telegram and checksum to autopilot
	auto_pilot_.Run(apb_telegram_with_checksum);

	//Send VTG telegram and checksum to autopilot
	

	//THIS MUST BE DEALT WITH. If one pipeline for autopilot telegrams is used, ~2200 lines of code must be debugged for
	//Navigation...If this change is made, this function must be mocked in every existing test, but can then also be tested
	//on.
}

//Called by PerformTask
void Navigation::stop() const
{
	//Stop the boat
	auto_pilot_.Stop();
}
