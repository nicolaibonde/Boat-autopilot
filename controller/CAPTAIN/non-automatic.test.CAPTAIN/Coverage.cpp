#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include "boost/fakeit.hpp"
#include "CAPTAIN/IGPS.h"
#include "CAPTAIN/IGPIO.h"
#include "CAPTAIN/TargetPosition.h"
#include "CAPTAIN/DCMotor.h"
#include "CAPTAIN/Servo.h"
#include "CAPTAIN/Autopilot.h"
#include "CAPTAIN/JSONTransmitter.h"
#include "CAPTAIN/Navigation.h"
#include "CAPTAIN/JSONReceiver.h"

//THIS TEST TAKES A VERY LONG TIME, UNCOMMENT TO RUN THE OTHER TESTS!!
//This test will verify that coverage_rectangle path will be plotted when Calculate is clicked. 
BOOST_AUTO_TEST_CASE(Coverage_Path_Calculation_from_56_10_to_56001_10_001_to_56002_10002)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<IGPIO> gpioMock;
	fakeit::Fake(Method(gpioMock, GpioServo));
	fakeit::Fake(Method(gpioMock, GpioSetMode));
	fakeit::Fake(Method(gpioMock, GpioHardwarePWM));

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	IGPIO& gpio = gpioMock.get();

	//Set function return values
	const Pose pose_start = Pose(Coordinate(56.150, 10.254), 30);
	const GPSStatus gps_status_start = GPSStatus(1.1, 2, 3.3, 4, pose_start);

	//Path to save file
	boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
		"website\\savedData\\");
	fakeit::When(Method(gpsMock, GetStatus)).Return(gps_status_start, gps_status_start);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_start);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(5.2);

	//Instantiate objects
	DCMotor dcmotor = DCMotor(gpio);
	Servo servo = Servo(gpio);
	Autopilot auto_pilot = Autopilot(servo, dcmotor);
	JSONTransmitter transmitter = JSONTransmitter(dcmotor, servo, gps, filepath.string());
	Navigation nav = Navigation(gps, transmitter, auto_pilot);
	JSONReceiver receiver = JSONReceiver(nav, auto_pilot, filepath.string());

	////Reset the fromNav file from a default file
	std::ifstream fromNavDefault(filepath.string() + "fromNavDefault.json");
	std::ofstream fromNavReset(filepath.string() + "fromNav.json", std::ofstream::out | std::ofstream::trunc);
	nlohmann::json from_nav_default_json;
	fromNavDefault >> from_nav_default_json;

	fromNavReset << from_nav_default_json.dump();
	fromNavDefault.close();
	fromNavReset.close();

	//Read from file to build string for Assert step
	std::ifstream toNav(filepath.string() + "toNav.json");
	nlohmann::json to_nav_json;
	toNav >> to_nav_json;

	while (to_nav_json.at("func_") == "none" || to_nav_json.at("func_") == "stop")
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "Waiting for calc..." << std::endl;
		toNav.clear();
		toNav.seekg(0, toNav.beg);
		toNav >> to_nav_json;
	}

	//Act - Setup and calculate a path
	{
		receiver.ReceiveActiveParam();
		receiver.ReceiveToNav();
	}	
}