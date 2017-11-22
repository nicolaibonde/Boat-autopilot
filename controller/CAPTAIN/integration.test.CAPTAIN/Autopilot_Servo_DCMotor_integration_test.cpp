#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "unit.test.CAPTAIN/boost/fakeit.hpp"
#include "CAPTAIN/Autopilot.h"
#include "CAPTAIN/IGPIO.h"
#include "CAPTAIN/Servo.h"
#include "CAPTAIN/DCMotor.h"

BOOST_AUTO_TEST_SUITE(Autopilot_Servo_DCMotor_integration_tests)

	BOOST_AUTO_TEST_CASE(construction_test)
	{

		fakeit::Mock<IGPIO> gpioMock;
		fakeit::Fake(Method(gpioMock, GpioServo));
		fakeit::Fake(Method(gpioMock, GpioSetMode));
		fakeit::Fake(Method(gpioMock, GpioPWM));
		fakeit::Fake(Method(gpioMock, GpioSetPWMfrequency));

		IGPIO & gpio = gpioMock.get();

		Servo servo(gpio);
		DCMotor thruster(gpio);

		Autopilot autopilot = Autopilot(servo, thruster);

		fakeit::Verify(Method(gpioMock, GpioSetPWMfrequency)).Exactly((fakeit::Once));
		fakeit::Verify(Method(gpioMock, GpioSetMode)).Exactly(2);

		fakeit::Verify(Method(gpioMock, GpioServo).Matching([](auto gpio, auto pulsewidth)
		{
			//Ignore the gpio, since it could change and is not important
			return pulsewidth == 1500;
		})).Exactly(1); //It should call it once in the constructor and once again when we set it.

		fakeit::Verify(Method(gpioMock, GpioPWM).Matching([](auto gpio, auto pulsewidth)
		{
			//Ignore the gpio, since it could change and is not important
			return pulsewidth == 0;
		})).Exactly(1); //It should call it once in the constructor and once again when we set it.
	}

	BOOST_AUTO_TEST_CASE(stop_autopilot)
	{

		fakeit::Mock<IGPIO> gpioMock;
		fakeit::Fake(Method(gpioMock, GpioServo));
		fakeit::Fake(Method(gpioMock, GpioSetMode));
		fakeit::Fake(Method(gpioMock, GpioPWM));
		fakeit::Fake(Method(gpioMock, GpioSetPWMfrequency));

		IGPIO & gpio = gpioMock.get();

		Servo servo(gpio);
		DCMotor thruster(gpio);

		Autopilot autopilot = Autopilot(servo, thruster);

		autopilot.Stop();

		fakeit::Verify(Method(gpioMock, GpioServo).Matching([](auto gpio, auto pulsewidth)
		{
			//Ignore the gpio, since it could change and is not important
			return pulsewidth == 1500;
		})).Exactly(2); //It should call it once in the constructor and once again when we set it.

		fakeit::Verify(Method(gpioMock, GpioPWM).Matching([](auto gpio, auto pulsewidth)
		{
			//Ignore the gpio, since it could change and is not important
			return pulsewidth == 0;
		})).Exactly(2); //It should call it once in the constructor and once again when we set it.
	}

	BOOST_AUTO_TEST_CASE(run_apb_autopilot)
	{

		fakeit::Mock<IGPIO> gpioMock;
		fakeit::Fake(Method(gpioMock, GpioServo));
		fakeit::Fake(Method(gpioMock, GpioSetMode));
		fakeit::Fake(Method(gpioMock, GpioPWM));
		fakeit::Fake(Method(gpioMock, GpioSetPWMfrequency));

		IGPIO & gpio = gpioMock.get();

		Servo servo(gpio);
		DCMotor thruster(gpio);

		Autopilot autopilot = Autopilot(servo, thruster);
		autopilot.SetParameters(1, 0, 0);


		std::string telegram= "$GPAPB,A,A,0.10,R,N,V,V,011,M,DEST,011,M,011,M*3C";


		autopilot.Run(telegram);

		fakeit::Verify(Method(gpioMock, GpioServo)).Exactly(2); //It should call it once in the constructor and once again when we set it.
	}

	BOOST_AUTO_TEST_CASE(run_both_autopilot)
	{

		fakeit::Mock<IGPIO> gpioMock;
		fakeit::Fake(Method(gpioMock, GpioServo));
		fakeit::Fake(Method(gpioMock, GpioSetMode));
		fakeit::Fake(Method(gpioMock, GpioPWM));
		fakeit::Fake(Method(gpioMock, GpioSetPWMfrequency));

		IGPIO & gpio = gpioMock.get();

		Servo servo(gpio);
		DCMotor thruster(gpio);

		Autopilot autopilot = Autopilot(servo, thruster);
		autopilot.SetParameters(1, 0, 0);


		std::string telegram = "$GPAPB,A,A,0.10,R,N,V,V,011,M,DEST,011,M,011,M*3C";
		autopilot.Run(telegram);
		telegram = "$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48";
		autopilot.Run(telegram);

		fakeit::Verify(Method(gpioMock, GpioServo)).Exactly(2); //It should call it once in the constructor and once again when we set it.
		fakeit::Verify(Method(gpioMock, GpioPWM)).Exactly(2);
	}
	
BOOST_AUTO_TEST_SUITE_END()