#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "CAPTAIN/Autopilot.h"
#include "boost/fakeit.hpp"

BOOST_AUTO_TEST_SUITE(Autopilot_tests)

	BOOST_AUTO_TEST_CASE(autopilot_test_stop)
	{
		fakeit::Mock<IPositionMotor> servoMock;
		fakeit::Fake(Method(servoMock, SetPosition));
		
		fakeit::Mock<ISpeedMotor> thrusterMock;
		fakeit::Fake(Method(thrusterMock, SetSpeed));

		IPositionMotor & servo = servoMock.get();
		ISpeedMotor & thruster = thrusterMock.get();

		Autopilot uut = Autopilot(servo, thruster);

		uut.Stop();

		fakeit::Verify(Method(servoMock, SetPosition).Matching([](auto pos)
		{
			//Ignore the gpio, since it could change and is not important
			return pos == 50;
		})).AtLeastOnce(); //It should call it once in the constructor and once again when we set it.

		fakeit::Verify(Method(thrusterMock, SetSpeed).Matching([](auto speed)
		{
			//Ignore the gpio, since it could change and is not important
			return speed == 0;
		})).AtLeastOnce(); //It should call it once in the constructor and once again when we set it.
	}

	BOOST_AUTO_TEST_CASE(autopilot_test_run_rudder)
	{
		fakeit::Mock<IPositionMotor> servoMock;
		fakeit::Fake(Method(servoMock, SetPosition));

		fakeit::Mock<ISpeedMotor> thrusterMock;
		fakeit::Fake(Method(thrusterMock, SetSpeed));

		IPositionMotor & servo = servoMock.get();
		ISpeedMotor & thruster = thrusterMock.get();

		Autopilot uut = Autopilot(servo, thruster);
		uut.SetParameters(1, 0, 0);


		std::string telegram = "$GPAPB,A,A,0.10,R,N,V,V,011,M,DEST,011,M,011,M*3C";

		uut.Run(telegram);

		fakeit::Verify(Method(servoMock, SetPosition)).AtLeastOnce();
		fakeit::Verify(Method(thrusterMock, SetSpeed)).Never();
	}
	
	BOOST_AUTO_TEST_CASE(autopilot_test_run_thruster)
	{
		fakeit::Mock<IPositionMotor> servoMock;
		fakeit::Fake(Method(servoMock, SetPosition));

		fakeit::Mock<ISpeedMotor> thrusterMock;
		fakeit::Fake(Method(thrusterMock, SetSpeed));

		IPositionMotor & servo = servoMock.get();
		ISpeedMotor & thruster = thrusterMock.get();

		Autopilot uut = Autopilot(servo, thruster);
		uut.SetParameters(1, 0, 0);

		std::string telegram = "$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48";

		uut.Run(telegram);

		fakeit::Verify(Method(servoMock, SetPosition)).Never();
		fakeit::Verify(Method(thrusterMock, SetSpeed)).AtLeastOnce();

	}

	BOOST_AUTO_TEST_CASE(autopilot_test_run_without_parameters)
	{
		fakeit::Mock<IPositionMotor> servoMock;
		fakeit::Fake(Method(servoMock, SetPosition));

		fakeit::Mock<ISpeedMotor> thrusterMock;
		fakeit::Fake(Method(thrusterMock, SetSpeed));

		IPositionMotor & servo = servoMock.get();
		ISpeedMotor & thruster = thrusterMock.get();

		Autopilot uut = Autopilot(servo, thruster);

		std::string telegram = "$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48";

		uut.Run(telegram);

		fakeit::Verify(Method(servoMock, SetPosition)).Never();
		fakeit::Verify(Method(thrusterMock, SetSpeed)).Never();

	}

	BOOST_AUTO_TEST_CASE(autopilot_test_run_checksum_wrong)
	{
		fakeit::Mock<IPositionMotor> servoMock;
		fakeit::Fake(Method(servoMock, SetPosition));

		fakeit::Mock<ISpeedMotor> thrusterMock;
		fakeit::Fake(Method(thrusterMock, SetSpeed));

		IPositionMotor & servo = servoMock.get();
		ISpeedMotor & thruster = thrusterMock.get();

		Autopilot uut = Autopilot(servo, thruster);
		uut.SetParameters(1, 0, 0);

		std::string telegram = "$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*50";

		uut.Run(telegram);

		fakeit::Verify(Method(servoMock, SetPosition)).Never();
		fakeit::Verify(Method(thrusterMock, SetSpeed)).Never();

	}

	BOOST_AUTO_TEST_CASE(autopilot_test_run_unknown_telegram)
	{
		fakeit::Mock<IPositionMotor> servoMock;
		fakeit::Fake(Method(servoMock, SetPosition));

		fakeit::Mock<ISpeedMotor> thrusterMock;
		fakeit::Fake(Method(thrusterMock, SetSpeed));

		IPositionMotor & servo = servoMock.get();
		ISpeedMotor & thruster = thrusterMock.get();

		Autopilot uut = Autopilot(servo, thruster);
		uut.SetParameters(1, 0, 0);

		std::string telegram = "$GPUKN,054.7,T,034.4,M,005.5,N,010.2,K*50";

		uut.Run(telegram);

		fakeit::Verify(Method(servoMock, SetPosition)).Never();
		fakeit::Verify(Method(thrusterMock, SetSpeed)).Never();

	}

BOOST_AUTO_TEST_SUITE_END()