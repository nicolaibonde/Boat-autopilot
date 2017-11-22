#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "CAPTAIN/Autopilot.h"
#include "unit.test.CAPTAIN/boost/fakeit.hpp"
#include "CAPTAIN/IGPS.h"
#include "CAPTAIN/IMotorStatusGetter.h"
#include "CAPTAIN/IAutopilot.h"
#include "CAPTAIN/Navigation.h"
#include "CAPTAIN/JSONTransmitter.h"
#include "CAPTAIN/JSONReceiver.h"

BOOST_AUTO_TEST_SUITE(JSONReceiver_Navigation_JSONTransmitter_tests)

//This test will verify that the navigation can generate a path from a manual call to PerformTask, and then that the
//transmitter will create the correct JSON file output
BOOST_AUTO_TEST_CASE(Navigate_Transmit_ServoPercentage)
{		
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<IAutopilot> autopilotMock;
	fakeit::Mock<IMotorStatusGetter> dcMock;
	fakeit::Mock<IMotorStatusGetter> servoMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();
	IMotorStatusGetter& dcMotor = dcMock.get();
	IMotorStatusGetter& servo = servoMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	//Path to save file
	boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
		"JSON\\Integration_tests\\NavTransmit_Servo\\");
	fakeit::When(Method(dcMock, GetStatus)).AlwaysReturn(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).AlwaysReturn(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).AlwaysReturn(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).AlwaysReturn(Pose(Coordinate(56.0, 10.0), 7.45));
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(5.2);

	//Instantiate objects
	JSONTransmitter transmitter = JSONTransmitter(dcMotor, servo, gps, filepath.string());
	Navigation nav = Navigation(gps, transmitter, auto_pilot);

	//Act - Save the file to the specified folder
	nav.PerformTask(CalcP2P,target);

	//Read from file to build string for Assert step
	std::ifstream fromNav(filepath.string() + "fromNav.json");
	nlohmann::json from_nav_json;
	fromNav >> from_nav_json;

	//Create expected string
	const std::string expected = "45.44";
	std::string from_nav_result;

	//Extract string from nested object
	for (nlohmann::json::iterator it = from_nav_json.at("Status_").begin(); it != from_nav_json.at("Status_").end(); ++it)
	{
		if (it.value().at("title_") == "Position")
		{
			for (nlohmann::json::iterator it2 = from_nav_json.at("Status_").at(it - from_nav_json.at("Status_").begin()).at("items_").begin();
				it2 != from_nav_json.at("Status_").at(it - from_nav_json.at("Status_").begin()).at("items_").end(); ++it2)
			{
				const double from_nav_result_double = it2.value().at("data_");
				std::ostringstream from_nav_result_stringstream;
				from_nav_result_stringstream << from_nav_result_double;
				from_nav_result = from_nav_result_stringstream.str();
			}
		}
	}

	//Assert
	BOOST_REQUIRE(from_nav_result == expected);
}

//This test will verify that the receiver can read a command with "CoverageRectangle", the navigation will generate a path,
//and the transmitter will create the correct JSON file output
BOOST_AUTO_TEST_CASE(Receive_Navigate)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const Pose pose = Pose(Coordinate(56.0, 10.0), 30);
	const GPSStatus gps_status = GPSStatus(0, 0, 0, 0, pose);
	boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
		"JSON\\Integration_tests\\ReceiveNav_CalcCoverageRectangle\\");
	fakeit::When(Method(gpsMock, GetPose)).AlwaysReturn(pose);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(gpsMock, GetStatus)).AlwaysReturn(gps_status);
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Instantiate objects
	Navigation nav = Navigation(gps, transmitter, auto_pilot);
	JSONReceiver receiver = JSONReceiver(nav, auto_pilot, filepath.string());
	
	//Act
	receiver.ReceiveToNav();
	NavigationData nav_data = nav.GetNavData();

	//Assert
	BOOST_REQUIRE(
		nav_data.Path_.front().Latitude_ == 56 && nav_data.Path_.front().Longitude_ == 10
	);
}

//This test will verify that the receiver can read a command with "CalcP2P", the navigation will generate a path,
//and the transmitter will create the correct JSON file output
BOOST_AUTO_TEST_CASE(Receive_Navigate_Transmit_CalcP2PPath)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<IAutopilot> autopilotMock;
	fakeit::Mock<IMotorStatusGetter> dcMock;
	fakeit::Mock<IMotorStatusGetter> servoMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();
	IMotorStatusGetter& dcMotor = dcMock.get();
	IMotorStatusGetter& servo = servoMock.get();
	
	//Set function return values
	const Pose pose = Pose(Coordinate(56.0, 10.0), 30);
	const GPSStatus gps_status = GPSStatus(0, 0, 0, 0, pose);
	const MotorStatus dc_status = MotorStatus(50.0, SPEED);
	const MotorStatus servo_status = MotorStatus(50.0, POSITION);
	boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
		"JSON\\Integration_tests\\ReceiveNavTransmit_CalcP2PPath\\");
	fakeit::When(Method(gpsMock, GetPose)).AlwaysReturn(pose);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(gpsMock, GetStatus)).AlwaysReturn(gps_status);
	fakeit::When(Method(dcMock, GetStatus)).AlwaysReturn(dc_status);
	fakeit::When(Method(servoMock, GetStatus)).AlwaysReturn(servo_status);

	//Instantiate objects
	JSONTransmitter transmitter = JSONTransmitter(dcMotor, servo, gps, filepath.string());
	Navigation nav = Navigation(gps, transmitter, auto_pilot);
	JSONReceiver receiver = JSONReceiver(nav, auto_pilot, filepath.string());
	
	//Act
	receiver.ReceiveToNav();

	//Assert
	//Could make all sorts of assertions, instead I think it's more sensible to take the Path_ output and "plot" in
	//https://www.darrinward.com/lat-long/?id=5a0cfac06a0d48.92366591
}

//This test will verify that the receiver can read a command with "CalcCoverage", the navigation will generate a path,
//and the transmitter will create the correct JSON file output
BOOST_AUTO_TEST_CASE(Receive_Navigate_Transmit_CoverageRectanglePath)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<IAutopilot> autopilotMock;
	fakeit::Mock<IMotorStatusGetter> dcMock;
	fakeit::Mock<IMotorStatusGetter> servoMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();
	IMotorStatusGetter& dcMotor = dcMock.get();
	IMotorStatusGetter& servo = servoMock.get();

	//Set function return values
	const Pose pose = Pose(Coordinate(56.0, 10.0), 30);
	const GPSStatus gps_status = GPSStatus(0, 0, 0, 0, pose);
	const MotorStatus dc_status = MotorStatus(50.0, SPEED);
	const MotorStatus servo_status = MotorStatus(50.0, POSITION);
	boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
		"JSON\\Integration_tests\\ReceiveNavTransmit_CalcCoverageRectangle\\");
	fakeit::When(Method(gpsMock, GetPose)).AlwaysReturn(pose);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(gpsMock, GetStatus)).AlwaysReturn(gps_status);
	fakeit::When(Method(dcMock, GetStatus)).AlwaysReturn(dc_status);
	fakeit::When(Method(servoMock, GetStatus)).AlwaysReturn(servo_status);

	//Instantiate objects
	JSONTransmitter transmitter = JSONTransmitter(dcMotor, servo, gps, filepath.string());
	Navigation nav = Navigation(gps, transmitter, auto_pilot);
	JSONReceiver receiver = JSONReceiver(nav, auto_pilot, filepath.string());

	//Act
	receiver.ReceiveToNav();

	//Assert
	//Could make all sorts of assertions, instead I think it's more sensible to take the Path_ output and "plot" in
	//https://www.darrinward.com/lat-long/?id=5a0cfac06a0d48.92366591
}

BOOST_AUTO_TEST_SUITE_END()