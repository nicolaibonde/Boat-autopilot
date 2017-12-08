//#define BOOST_TEST_DYN_LINK
//#ifdef STAND_ALONE
//#   define BOOST_TEST_MODULE Main
//#endif
//#include <boost/test/unit_test.hpp>
//#include "boost/fakeit.hpp"
//#include <boost/filesystem.hpp>
//#include "CAPTAIN/IGPS.h"
//#include "CAPTAIN/IGPIO.h"
//#include "CAPTAIN/TargetPosition.h"
//#include "CAPTAIN/DCMotor.h"
//#include "CAPTAIN/Servo.h"
//#include "CAPTAIN/Autopilot.h"
//#include "CAPTAIN/JSONTransmitter.h"
//#include "CAPTAIN/Navigation.h"
//#include "CAPTAIN/JSONReceiver.h"
//
////THIS TEST TAKES A VERY LONG TIME, UNCOMMENT TO RUN THE OTHER TESTS!!
////This test will verify the full functionality of the P2P calculation and traversal with the website. 
//BOOST_AUTO_TEST_CASE(P2P_Path_Calculation_and_traversal_from_56_10_to_56001_10001)
//{
//	//Arrange
//	//Link mocks to interfaces
//	fakeit::Mock<IGPS> gpsMock;
//	fakeit::Mock<IGPIO> gpioMock;
//	fakeit::Fake(Method(gpioMock, GpioServo));
//	fakeit::Fake(Method(gpioMock, GpioSetMode));
//	fakeit::Fake(Method(gpioMock, GpioHardwarePWM));
//
//	//Instantiate mocks
//	IGPS& gps = gpsMock.get();
//	IGPIO& gpio = gpioMock.get();
//
//	//Set function return values
//	const TargetPosition target = TargetPosition({ 56.001, 10.001 });
//
//	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
//	const GPSStatus gps_status_start = GPSStatus(1.1, 2, 3.3, 4, pose_start);
//	const Pose pose_1 = Pose(Coordinate(56.0, 10.0), 30);
//	const GPSStatus gps_status_1 = GPSStatus(1.1, 2, 3.3, 4, pose_1);
//	const Pose pose_2 = Pose(Coordinate(56.000077, 10.000077), 30);
//	const GPSStatus gps_status_2 = GPSStatus(1.1, 2, 3.3, 4, pose_2);
//	const Pose pose_3 = Pose(Coordinate(56.000154, 10.000154), 30);
//	const GPSStatus gps_status_3 = GPSStatus(1.1, 2, 3.3, 4, pose_3);
//	const Pose pose_4 = Pose(Coordinate(56.000231, 10.000231), 30);
//	const GPSStatus gps_status_4 = GPSStatus(1.1, 2, 3.3, 4, pose_4);
//	const Pose pose_5 = Pose(Coordinate(56.000308, 10.000308), 30);
//	const GPSStatus gps_status_5 = GPSStatus(1.1, 2, 3.3, 4, pose_5);
//	const Pose pose_6 = Pose(Coordinate(56.000385, 10.000385), 30);
//	const GPSStatus gps_status_6 = GPSStatus(1.1, 2, 3.3, 4, pose_6);
//	const Pose pose_7 = Pose(Coordinate(56.000462, 10.000462), 30);
//	const GPSStatus gps_status_7 = GPSStatus(1.1, 2, 3.3, 4, pose_7);
//	const Pose pose_8 = Pose(Coordinate(56.000538, 10.000538), 30);
//	const GPSStatus gps_status_8 = GPSStatus(1.1, 2, 3.3, 4, pose_8);
//	const Pose pose_9 = Pose(Coordinate(56.000615, 10.000615), 30);
//	const GPSStatus gps_status_9 = GPSStatus(1.1, 2, 3.3, 4, pose_9);
//	const Pose pose_10 = Pose(Coordinate(56.000692, 10.000692), 30);
//	const GPSStatus gps_status_10 = GPSStatus(1.1, 2, 3.3, 4, pose_10);
//	const Pose pose_11 = Pose(Coordinate(56.000769, 10.000769), 30);
//	const GPSStatus gps_status_11 = GPSStatus(1.1, 2, 3.3, 4, pose_11);
//	const Pose pose_12 = Pose(Coordinate(56.000846, 10.000846), 30);
//	const GPSStatus gps_status_12 = GPSStatus(1.1, 2, 3.3, 4, pose_12);
//	const Pose pose_13 = Pose(Coordinate(56.000923, 10.000923), 30);
//	const GPSStatus gps_status_13 = GPSStatus(1.1, 2, 3.3, 4, pose_13);
//	const Pose pose_end = Pose(Coordinate(56.001, 10.001), 30);
//	const GPSStatus gps_status_end = GPSStatus(1.1, 2, 3.3, 4, pose_end);
//
//	//Path to save file
//	boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
//		"website\\savedData\\");
//	fakeit::When(Method(gpsMock, GetStatus)).Return(gps_status_start, gps_status_start, gps_status_1, gps_status_2, gps_status_3,
//		gps_status_4, gps_status_5, gps_status_6, gps_status_7, gps_status_8, gps_status_9, gps_status_10, gps_status_11, gps_status_12,
//		gps_status_13, gps_status_end);
//	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_start, pose_start, pose_start, pose_start, pose_1, pose_1, pose_1,
//		pose_2, pose_2, pose_2, pose_3, pose_3, pose_3, pose_4, pose_4, pose_4, pose_5, pose_5, pose_5, pose_6, pose_6, pose_6, pose_7,
//		pose_7, pose_7, pose_8, pose_8, pose_8, pose_9, pose_9, pose_9, pose_10, pose_10, pose_10, pose_11, pose_11, pose_11, pose_12,
//		pose_12, pose_12, pose_13, pose_13, pose_13, pose_end, pose_end, pose_end);
//	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(5.2);
//
//	//Instantiate objects
//	DCMotor dcmotor = DCMotor(gpio);
//	Servo servo = Servo(gpio);
//	Autopilot auto_pilot = Autopilot(servo, dcmotor);
//	JSONTransmitter transmitter = JSONTransmitter(dcmotor, servo, gps, filepath.string());
//	Navigation nav = Navigation(gps, transmitter, auto_pilot);
//	JSONReceiver receiver = JSONReceiver(nav, auto_pilot, filepath.string());
//
//	////Reset the fromNav file from a default file
//	std::ifstream fromNavDefault(filepath.string() + "fromNavDefault.json");
//	std::ofstream fromNavReset(filepath.string() + "fromNav.json", std::ofstream::out | std::ofstream::trunc);
//	nlohmann::json from_nav_default_json;
//	fromNavDefault >> from_nav_default_json;
//
//	fromNavReset << from_nav_default_json.dump();
//	fromNavDefault.close();
//	fromNavReset.close();
//
//	//Read from file to build string for Assert step
//	std::ifstream toNav(filepath.string() + "toNav.json");
//	nlohmann::json to_nav_json;
//	toNav >> to_nav_json;
//
//	while (to_nav_json.at("func_") == "none" || to_nav_json.at("func_") == "stop")
//	{
//		std::this_thread::sleep_for(std::chrono::seconds(1));
//		std::cout << "Waiting for calc..." << std::endl;
//		toNav.clear();
//		toNav.seekg(0, toNav.beg);
//		toNav >> to_nav_json;
//	}
//
//	//Act - First setup and calculate a path -
//	{
//		receiver.ReceiveActiveParam();
//		receiver.ReceiveToNav();
//		std::this_thread::sleep_for(std::chrono::seconds(1));
//	}
//
//	while (to_nav_json.at("func_") == "calcP2P")
//	{
//		std::this_thread::sleep_for(std::chrono::seconds(1));
//		std::cout << "Waiting for start..." << std::endl;
//		toNav.clear();
//		toNav.seekg(0, toNav.beg);
//		toNav >> to_nav_json;
//	}
//
//	//then run the loop to traverse the full path
//	for (int i = 1; i < 16; i++)
//	{
//		//nav.PerformTask(Start);
//		receiver.ReceiveToNav();
//		std::cout << "Reached point " << i << "..." << std::endl;
//		std::this_thread::sleep_for(std::chrono::seconds(1));
//	}
//
//	//Get data from nav and check that the path has been completed.
//	NavigationData nav_data = nav.GetNavData();
//
//	//Verify that the path has been completed
//	BOOST_REQUIRE(nav_data.Progress_ == 100 && nav_data.Ete_ == 0 && nav_data.Path_.size() == 14 && nav_data.Completed_path_.size() == 14);
//}