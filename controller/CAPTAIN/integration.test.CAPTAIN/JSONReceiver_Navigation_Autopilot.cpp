#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "CAPTAIN/Autopilot.h"
#include "unit.test.CAPTAIN/boost/fakeit.hpp"
#include "CAPTAIN/IGPS.h"
#include "CAPTAIN/IMotorStatusGetter.h"
#include "CAPTAIN/Navigation.h"
#include "CAPTAIN/JSONTransmitter.h"
#include "CAPTAIN/JSONReceiver.h"
#include "CAPTAIN/DCMotor.h"
#include "CAPTAIN/Servo.h"

BOOST_AUTO_TEST_SUITE(JSONReceiver_Navigation_Autopilot_tests)

	//This test will verify that the receiver can set parameters in autopilot, then read the toNav.json file with CalcP2P. A second,
	//faked, call to PerformTask in navigation will then verify that the motors are called from the autopilot.
	BOOST_AUTO_TEST_CASE(Navigate_Transmit_Motors_called)
	{
		//Arrange
		//Link mocks to interfaces
		fakeit::Mock<IGPS> gpsMock;
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IPositionMotor> positionMock;
		fakeit::Mock<ISpeedMotor> speedMock;

		//Instantiate mocks
		IGPS& gps = gpsMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IPositionMotor& position = positionMock.get();
		ISpeedMotor& speed = speedMock.get();

		//Set function return values
		const TargetPosition target = TargetPosition({ 56.01, 10.01 });

		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Integration_tests\\ReceiveNavAutopilot_Motors_called\\");
		fakeit::When(Method(dcMock, GetStatus)).AlwaysReturn(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).AlwaysReturn(MotorStatus(45.44, POSITION));
		fakeit::Fake(Method(speedMock, SetSpeed));
		fakeit::Fake(Method(positionMock, SetPosition));
		fakeit::When(Method(gpsMock, GetStatus)).AlwaysReturn(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).AlwaysReturn(Pose(Coordinate(56.0, 10.0), 7.45));
		fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(5.2);

		//Instantiate objects
		Autopilot auto_pilot = Autopilot(position, speed);
		JSONTransmitter transmitter = JSONTransmitter(dcMotor, servo, gps, filepath.string());
		Navigation nav = Navigation(gps, transmitter, auto_pilot);
		JSONReceiver receiver = JSONReceiver(nav, auto_pilot, filepath.string());

		//Act - Set parameters for autopilot, then read the toNav.json file. 
		//Finally, a faked second read of the JSON file, now with Start.
		receiver.ReceiveActiveParam();
		receiver.ReceiveToNav();
		nav.PerformTask(Start);

		//Assert
		fakeit::Verify(Method(speedMock, SetSpeed)).Exactly(1);
		fakeit::Verify(Method(positionMock, SetPosition)).Exactly(1);
	}

BOOST_AUTO_TEST_SUITE_END()