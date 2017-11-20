#include "GPSStatus.h"
#include "json.hpp"
#include <chrono>


GPSStatus::GPSStatus(const double fix, const int satellites, const double hdop, const int fix_timestamp,
	const Pose pose) : pose_(pose)
{
	fix_ = fix;
	satellites_ = satellites;
	hdop_ = hdop;
	fix_timestamp_ = fix_timestamp;
}

GPSStatus::~GPSStatus()
{
}

std::string GPSStatus::GetString()
{
	nlohmann::json connection = {
		{"title_", "GPS Information"},
		{"items_", {
			{
				{"data_",fix_},
				{"title_","GPS fix"},
				{"unit_",""}
			},
			{ 
				{ "data_",satellites_ },
				{ "title_","Satelites" },
				{ "unit_","" } 
			} ,
			{
				{ "data_",hdop_ },
				{ "title_","Horizontal dilution of precision" },
				{ "unit_","" }
			},
			{
				{ "data_",fix_timestamp_ },
				{ "title_","Fix timestamp" },
				{ "unit_","UTC" }
			}

		}
		}
	};

	nlohmann::json pose = {
		{ "title_", "Pose" },
		{ "items_",{
			{
				{ "data_",pose_.Coordinate_.Latitude_ },
				{ "title_","Latitude" },
				{ "unit_","deg" }
			},
			{
				{ "data_",pose_.Coordinate_.Longitude_ },
				{ "title_","Longitude" },
				{ "unit_","deg" }
			},
			{
				{ "data_",pose_.Orientation_},
				{ "title_","Orientation" },
				{ "unit_","deg" }
			}
		}
		}
	};

	std::string result = connection.dump() + "," + pose.dump();

	return  result;
}
