#pragma once
#include <fstream>
#include <string>
#include "json.hpp"
#include "JSONTransmitter.h"
#include "INavigation.h"

JSONTransmitter::JSONTransmitter(IMotorStatusGetter& dc_motor, IMotorStatusGetter& servo,
	IGPS& gps, const std::string filepath) : dc_motor_(dc_motor), servo_(servo), gps_(gps)
{
	//Object references must be initialized in the initializer list above
	filepath_ = filepath;
}

JSONTransmitter::~JSONTransmitter()
{
	//Object is persistent - only destroyed at program termination
}

void JSONTransmitter::NavAcquisition(INavigation* navigation)
{
	//Acquires a pointer to an INavigation object
	navigation_ = navigation;
}

void JSONTransmitter::TransmitFromNav(std::string const timestamp_in)
{	
	//Construction of nav_data object by invoking GetNavaData on &INavigation reference object
	NavigationData nav_data = navigation_->GetNavData();
	
	//Extraction of completed path and path vectors
	const std::vector<Coordinate> completed_path_coordinates_vector = nav_data.Completed_path_;
	const std::vector<Coordinate> path_coordinates_vector = nav_data.Path_;

	//Construction of MotorStatus object by invoking GetStatus on &IMotorStatusGetter reference object
	//and subsequent extraction of status string by invoking GetString on the MotorStatus object
	const std::string ms_string  = dc_motor_.GetStatus().GetString();

	//Construction of MotorStatus object by invoking GetStatus on &IMotorStatusGetter reference object
	//and subsequent extraction of status string by invoking GetString on the MotorStatus object
	const std::string ss_string = servo_.GetStatus().GetString();

	//Construction of GPSStatus object by invoking GetStatus on &IGPS reference object
	GPSStatus gps_status = gps_.GetStatus();
	//Extraction of status string by invoking GetString on GPSStatus object
	const std::string gpss_string = gps_status.GetString();
	//Construction of Pose object by invoking GetPose on &IGPS reference object
	Pose gps_pose = gps_.GetPose();

	//Construction of status string
	const std::string status_str = "[" + gpss_string + "," + ms_string + "," + ss_string + "]";

	//Construction of timestamp
	const std::string timestamp = timestamp_in;

	//format ete to a text string, eg. an ete of (int) 185 becomes (std::string) "3 min 5 secs"
	std::string ete = formatEte(nav_data.Ete_);

	nlohmann::json progress;

	std::string iterator_completed_path_string;

	if (completed_path_coordinates_vector.size() > 1)
	{
		//Construction of completed path JS object
		iterator_completed_path_string = formatPathString(completed_path_coordinates_vector);

		//Fix completed path JS object ending
		iterator_completed_path_string = iterator_completed_path_string.substr(0,
			iterator_completed_path_string.length() - 1);
		iterator_completed_path_string.append("]}");

		//Construction of progress JS object
		progress = {
			{ "ete_", ete },
			{ "percentage_", nav_data.Progress_ }
		};
	}
	else
	{
		iterator_completed_path_string = R"({"line_":[]})";

		//Construction of progress JS object
		progress = {
			{ "ete_", "Calculating ..." },
			{ "percentage_", "0.0" }
		};
	}

	std::string iterator_path_string;

	if (path_coordinates_vector.size() >= 2)
	{
		//Construction of completed path JS object
		iterator_path_string = formatPathString(path_coordinates_vector);

		//Fix path JS object ending
		iterator_path_string = iterator_path_string.substr(0,
			iterator_path_string.length() - 1);
		iterator_path_string.append(R"(],"timestamp_": )");
		iterator_path_string.append(timestamp);
		iterator_path_string.append("}");
	}
	else
	{
		iterator_path_string = R"({"line_":[]})";
	}

	//Construction of telemetry JS object
	const nlohmann::json telemetry = {
		{"latitude_", (gps_pose.Coordinate_.Latitude_)},
		{"longitude_", (gps_pose.Coordinate_.Longitude_)},
		{"orientation_", (gps_pose.Orientation_)}
	};

	//Construction of fromNav JS object
	nlohmann::json fromNav;
	
	fromNav["Completed_path_"] = nlohmann::json::parse(iterator_completed_path_string);
	fromNav["Path_"] = nlohmann::json::parse(iterator_path_string);
	fromNav["Status_"] = nlohmann::json::parse(status_str);
	fromNav["Telemetry_"] = telemetry;
	fromNav["Progress_"] = progress;
	fromNav["Timestamp_"] = nlohmann::json::parse(timestamp);
	
	//Dump of fromNav JS object to local file at the specified path
	std::ofstream file(filepath_ + "fromNav.json");
	file << fromNav.dump();
}

//Helper function to format ete to string
std::string JSONTransmitter::formatEte(const int ete) const
{	
	std::string ete_formatted;
	
	//Standard divide and floor to get minutes, then cast to const int
	const int ete_min = static_cast<const int>(floor(ete / (60*1000)));
	//Modulus, then cast to const int
	const int ete_sec = static_cast<const int>(round((ete/1000) % 60));

	//If 1 minute or more, put the minute count in the string. Otherwise just put seconds.
	if (ete_min >= 1)
		ete_formatted = std::to_string(ete_min) + " min " + std::to_string(ete_sec) + " sec";
	else
		ete_formatted = std::to_string(ete_sec) + " sec";
	
	return ete_formatted;
}
//Helper function to extract Latitudes and Longitudes from coordinate vector list and 
//package them appropriately for use with JSON.
std::string JSONTransmitter::formatPathString(std::vector<Coordinate> coordinates_vector) const
{
	std::string path_string = R"({"line_":[)";

	for (std::vector<Coordinate>::iterator it = coordinates_vector.begin();
		it != coordinates_vector.end(); ++it) {
		path_string.append("{");
		path_string.append(R"("latitude_": )");
		path_string.append(std::to_string(it->Latitude_));
		path_string.append(R"(,"longitude_": )");
		path_string.append(std::to_string(it->Longitude_));
		path_string.append("},");
	}

	return path_string;
}