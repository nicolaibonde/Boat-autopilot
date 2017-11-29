#include "Ublox_neo7m.h"
#include <boost/algorithm/string.hpp>
#include "GL_Rhumb.h"
#include "SimpleSerial.h"
#include <thread>
#include <iostream>

Ublox_neo7m::Ublox_neo7m(ISerial &serial) :
	serial_(serial),
	pose_(Coordinate(0, 0), 0),
	old_pose_(Coordinate(0, 0), 0),
	status_(-1, -1, -1, -1, pose_)
{
	speed_ = 0;
	thread_run_ = false;
}

Ublox_neo7m::~Ublox_neo7m()
{
}

std::thread Ublox_neo7m::Setup()
{
	thread_run_ = true;
	//Returns the thread that should be join'ed in the main thread, so it can get the gps date without blocking
	return std::thread(&Ublox_neo7m::getGPSData, this);
}

void Ublox_neo7m::StopThread()
{
	thread_run_ = false;
}

void Ublox_neo7m::getGPSData()
{
	while(thread_run_)
	{
		//Getting the telegram
		std::string telegram = serial_.ReadLine();


		//Hold a list of strings split by "," delimiter
		std::vector<std::string> splitTelegram;

		//Splitting the telegram on "," delimiter
		split(splitTelegram, telegram, boost::is_any_of(","));

		//Check if the checksum checks out
		if (checksum(telegram)) {
			//check if it is a GGA telgram and ignoring talker
			if (splitTelegram[0].find("GGA") != std::string::npos )
			{
				//Extract latitude and longitude
				double lat = convertDegreeMinutes2Degrees(splitTelegram[2]);
				double lon = convertDegreeMinutes2Degrees(splitTelegram[4]);
				old_pose_ = pose_;

				//Calculate the new pose from the old, this is to get the orientation
				pose_ = calculatePose(old_pose_.Coordinate_, Coordinate(lat, lon));

				
				//Extract data for status
				//Try catche
				double fix;
				try
				{
					fix = stod(splitTelegram[6]);
				}
				catch (...)
				{
					fix = 0;
				}

				int satellites;
				try
				{
					satellites = stoi(splitTelegram[7]);
				}
				catch (...)
				{
					satellites = 0;
				}

				double hdop;
				try
				{
					hdop = stod(splitTelegram[8]);
				}
				catch (...)
				{
					hdop = 0;
				}

				int fix_timestamp;
				try
				{
					fix_timestamp = stoi(splitTelegram[1]);
				}
				catch (...)
				{
					fix_timestamp = 0;
				}


				status_ = GPSStatus(fix, satellites, hdop, fix_timestamp, pose_);
			
			//Check if the telegram is VTG and ignore the talker 
			}else if(splitTelegram[0].find("VTG") != std::string::npos)
			{
				//Extract the speed in km/h from the telegram
				try
				{
					speed_ = stod(splitTelegram[7]);
				}
				catch (...)
				{
					speed_ = 0;
				}
				
			}
		}

	}
}

bool Ublox_neo7m::checksum(std::string telegram)
{ 
	//Check if it is a complete telegram
	if(telegram[0] == '$' && telegram[telegram.size()-3] == '*')
	{
		//extract the checksum hex number
		const std::string expected_checksum_string = telegram.substr(telegram.size() - 2, telegram.size() - 1);

		//Remove $ from beginning and checksum and * from end
		std::string bare_telegram = telegram.substr(1, telegram.size() - 4);

		//Calculate the checksum of the bare_telegram
		int result_checksum = 0;
		for (char& c : bare_telegram) {
			result_checksum = result_checksum ^ c;
		}

		//Convert the int checksum to a hex string 
		std::stringstream checksum_stream;
		checksum_stream << std::uppercase <<std::hex << result_checksum;
		std::string result_checksum_string(checksum_stream.str());

		//std::cout << result_checksum_string << std::endl;
		//Return if the expected checksum is the same as the calculated
		return result_checksum_string == expected_checksum_string;
	}
}

Pose Ublox_neo7m::GetPose()
{
	return pose_;
}

double Ublox_neo7m::GetSpeed()
{
	return speed_;
}

GPSStatus Ublox_neo7m::GetStatus()
{
	return status_;
}

double Ublox_neo7m::convertDegreeMinutes2Degrees(std::string degree_minutes) const
{
	//Find the . to be used as a fixed point
	const int delimtIndex = degree_minutes.find(".");
	
	//Extract ddd from start of string on til 2 before the .
	int ddd;
	try
	{
		ddd = std::stoi(degree_minutes.substr(0, delimtIndex - 2));
	}
	catch (...)
	{
		ddd = 0;
	}
	
	//The rest is mm
	double mm;
	try
	{
		mm = std::stod(degree_minutes.substr(delimtIndex - 2, degree_minutes.length() - 1));
	}
	catch (...)
	{
		mm = 0;
	}
	
	//return the convertion
	return ddd + mm / 60;
}

Pose Ublox_neo7m::calculatePose(const Coordinate old_coordinate, const Coordinate new_coordinate)
{
	//Define the WGS84 constants
	double a = GeographicLib::Constants::WGS84_a();
	double f = GeographicLib::Constants::WGS84_f();

	//Define a rhumb using the WGS84 constants
	GeographicLib::Rhumb rhumb(a, f);
	
	//Take the inverse of the defined rhumb with two coordinates; result distance and bearing are put in the last two arguments
	double dist, orientation;
	rhumb.Inverse(old_coordinate.Latitude_, old_coordinate.Longitude_, new_coordinate.Latitude_, new_coordinate.Longitude_, dist, orientation);
	
	//Check orientation and return pose
	return Pose(new_coordinate, orientation < 0 ? orientation + 360 : orientation);
}


