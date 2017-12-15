#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "CAPTAIN/Autopilot.h"
#include "unit.test.CAPTAIN/boost/fakeit.hpp"
#include "CAPTAIN/IGPS.h"
#include "CAPTAIN/Navigation.h"
#include "CAPTAIN/JSONTransmitter.h"
#include "CAPTAIN/JSONReceiver.h"
#include "CAPTAIN/DCMotor.h"
#include "CAPTAIN/Servo.h"

BOOST_AUTO_TEST_SUITE(JSONReceiver_Navigation_JSONTransmitter_Autopilot_Motors_tests)

//This test will verify that the receiver can set parameters in autopilot, then read the toNav.json file with CalcP2P. A second,
//faked, call to PerformTask in navigation will then verify that the motors call the GPIO pins the correct number of times
//and with the default values
BOOST_AUTO_TEST_CASE(Navigate_Transmit_GPIO_called)
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
	IGPIO & gpio = gpioMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });

	//Path to save file
	boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
		"JSON\\Integration_tests\\ReceiveNavAutopilotMotors_GPIO_called\\");
	fakeit::When(Method(gpsMock, GetStatus)).AlwaysReturn(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).AlwaysReturn(Pose(Coordinate(56.0, 10.0), 7.45));
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(5.2);

	//Instantiate objects
	DCMotor dcmotor = DCMotor(gpio);
	Servo servo = Servo(gpio);
	Autopilot auto_pilot = Autopilot(servo, dcmotor);
	JSONTransmitter transmitter = JSONTransmitter(dcmotor, servo, gps, filepath.string());
	Navigation nav = Navigation(gps, transmitter, auto_pilot);
	JSONReceiver receiver = JSONReceiver(nav, auto_pilot, filepath.string());

	//Act - Set parameters for autopilot, then read the toNav.json file. 
	//Finally, a faked second read of the JSON file, now with Start.
	receiver.ReceiveActiveParam();
	receiver.ReceiveToNav();
	nav.PerformTask(Start);

	//Assert
	fakeit::Verify(Method(gpioMock, GpioHardwarePWM)).Exactly(2);
	fakeit::Verify(Method(gpioMock, GpioSetMode)).Exactly(2);

	fakeit::Verify(Method(gpioMock, GpioServo).Matching([](auto gpio, auto pulsewidth)
	{
		//Ignore the gpio, since it could change and is not important
		return pulsewidth == 1500;
	})).Exactly(1); //It should call it once in the constructor and once again when we set it.

	fakeit::Verify(Method(gpioMock, GpioHardwarePWM).Matching([](auto gpio_pin, auto frequency, auto pulsewidth)
	{
		//Ignore the gpio, since it could change and is not important
		return pulsewidth == 0;
	})).Exactly(1); //It should call it once in the constructor and once again when we set it.
}

//This test will verify that the receiver can set parameters in autopilot, then read the toNav.json file with CalcP2P. Several other,
//faked, calls to PerformTask in navigation will then verify that the motors call the GPIO pins the correct number of times
//and with the correct values, as well as check the output of the JSONTransmitter file write.
BOOST_AUTO_TEST_CASE(Receive_Navigate_Transmit_Autopilot_Motors_GPIO_called_Transmitter_outputs) 
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<IGPIO> gpioMock;
	fakeit::Fake(Method(gpioMock, GpioServo));
	fakeit::Fake(Method(gpioMock, GpioSetMode));
	fakeit::Fake(Method(gpioMock, GpioPWM));
	fakeit::Fake(Method(gpioMock, GpioHardwarePWM));

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	IGPIO & gpio = gpioMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const GPSStatus gps_status_start = GPSStatus(1.1, 2, 3.3, 4, pose_start);
	const Pose pose_2 = Pose(Coordinate(56.000078, 10.000078), 30);
	const GPSStatus gps_status_2 = GPSStatus(1.1, 2, 3.3, 4, pose_2);
	const Pose pose_3 = Pose(Coordinate(56.000156, 10.000156), 30);
	const GPSStatus gps_status_3 = GPSStatus(1.1, 2, 3.3, 4, pose_3);

	//Path to save file
	boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
		"JSON\\Integration_tests\\ReceiveNavTransmitAutopilotMotors_GPIO_called_Transmitter_outputs\\");
	fakeit::When(Method(gpsMock, GetStatus)).Return(gps_status_start, gps_status_start, gps_status_2, gps_status_3);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_start, pose_start, pose_start, pose_start, pose_2, pose_2, pose_2, pose_3, pose_3, pose_3);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(5.2);

	//Instantiate objects
	DCMotor dcmotor = DCMotor(gpio);
	Servo servo = Servo(gpio);
	Autopilot auto_pilot = Autopilot(servo, dcmotor);
	JSONTransmitter transmitter = JSONTransmitter(dcmotor, servo, gps, filepath.string());
	Navigation nav = Navigation(gps, transmitter, auto_pilot);
	JSONReceiver receiver = JSONReceiver(nav, auto_pilot, filepath.string());

	//Act - Set parameters for autopilot, then read the toNav.json file. 
	//Finally, a faked second read of the JSON file, now with Start.
	receiver.ReceiveActiveParam();
	receiver.ReceiveToNav();
	nav.PerformTask(Start);
	nav.PerformTask(Start);
	nav.PerformTask(Start);

	//Assert
	fakeit::Verify(Method(gpioMock, GpioHardwarePWM)).Exactly(4);
	fakeit::Verify(Method(gpioMock, GpioServo)).Exactly(4);

	fakeit::Verify(Method(gpioMock, GpioServo).Matching([](auto gpio, auto pulsewidth)
	{
		//Ignore the gpio, since it could change and is not important
		return pulsewidth == 1500;
	})).Exactly(1); //It should call it once in the constructor and once again when we set it.

	fakeit::Verify(Method(gpioMock, GpioHardwarePWM).Matching([](auto gpio_pin, auto frequency, auto pulsewidth)
	{
		//Ignore the gpio, since it could change and is not important
		return pulsewidth == 0;
	})).Exactly(1); //It should call it once in the constructor and once again when we set it.

	//Read from file to build string for Assert step
	std::ifstream fromNav(filepath.string() + "fromNav.json");
	nlohmann::json from_nav_json;
	fromNav >> from_nav_json;

	//Create expected string
	const std::string expected = "35";
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

BOOST_AUTO_TEST_SUITE_END()