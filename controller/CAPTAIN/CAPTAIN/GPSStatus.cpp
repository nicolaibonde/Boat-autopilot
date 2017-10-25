#include "GPSStatus.h"
#include "json.hpp"


GPSStatus::GPSStatus(const double frequency, const double delay, const Pose pose) : pose_(pose)
{
	frequency_ = frequency;
	delay_ = delay;
}

GPSStatus::~GPSStatus()
{
}

std::string GPSStatus::GetString()
{
	nlohmann::json connection = {
		{"title_", "GPS Connection"},
		{"items_", {
			{
				{"data_",frequency_},
				{"title_","GPS frequency"},
				{"unit_","Hz"}
			},
			{ 
				{ "data_",delay_ },
				{ "tile_","GPS delay" },
				{ "unit_","ms" } 
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
