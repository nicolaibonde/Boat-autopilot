#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "CAPTAIN/Servo.h"
#include "boost/fakeit.hpp"
#include "CAPTAIN/IPositionMotor.h"

BOOST_AUTO_TEST_SUITE(Servo_tests)
	BOOST_AUTO_TEST_CASE(Servo_gpio_setup_test)
	{
		fakeit::Mock<IGPIO> gpioMock;
		fakeit::Fake(Method(gpioMock, GpioServo));
		fakeit::Fake(Method(gpioMock, GpioSetMode));
		
		IGPIO & gpio = gpioMock.get();

		Servo uut(gpio);

		fakeit::Verify(Method(gpioMock, GpioSetMode)).Exactly(fakeit::Once);
		//I dont think it make sense to check on what it is set to since that is hardware specific
		fakeit::Verify(Method(gpioMock, GpioServo)).Exactly(fakeit::Once);

	}

	BOOST_AUTO_TEST_CASE(Servo_test_SetPosition_50_returns_1500)
	{
		fakeit::Mock<IGPIO> gpioMock;
		fakeit::Fake(Method(gpioMock, GpioServo));
		fakeit::Fake(Method(gpioMock, GpioSetMode));

		IGPIO & gpio = gpioMock.get();

		Servo uut(gpio);

		uut.SetPosition(50);

		fakeit::Verify(Method(gpioMock, GpioServo).Matching([](auto gpio, auto pulsewidth)
		{
			//Ignore the gpio, since it could change and is not important
			return pulsewidth == 1500;
		})).Exactly(2); //It should call it once in the constructor and once again when we set it.

	}

	BOOST_AUTO_TEST_CASE(Servo_test_SetPosition_negative_10_returns_500)
	{
		fakeit::Mock<IGPIO> gpioMock;
		fakeit::Fake(Method(gpioMock, GpioServo));
		fakeit::Fake(Method(gpioMock, GpioSetMode));

		IGPIO & gpio = gpioMock.get();

		Servo uut(gpio);

		uut.SetPosition(-10);

		fakeit::Verify(Method(gpioMock, GpioServo).Matching([](auto gpio, auto pulsewidth)
		{
			//Ignore the gpio, since it could change and is not important
			return pulsewidth == 500;
		})); //It should call it once in the constructor and once again when we set it.

	}

	BOOST_AUTO_TEST_CASE(Servo_test_SetPosition_110_returns_2500)
	{
		fakeit::Mock<IGPIO> gpioMock;
		fakeit::Fake(Method(gpioMock, GpioServo));
		fakeit::Fake(Method(gpioMock, GpioSetMode));

		IGPIO & gpio = gpioMock.get();

		Servo uut(gpio);

		uut.SetPosition(110);

		fakeit::Verify(Method(gpioMock, GpioServo).Matching([](auto gpio, auto pulsewidth)
		{
			//Ignore the gpio, since it could change and is not important
			return pulsewidth == 2500;
		})); //It should call it once in the constructor and once again when we set it.

	}

	BOOST_AUTO_TEST_CASE(Servo_test_SetPosition_100_returns_2500)
	{
		fakeit::Mock<IGPIO> gpioMock;
		fakeit::Fake(Method(gpioMock, GpioServo));
		fakeit::Fake(Method(gpioMock, GpioSetMode));

		IGPIO & gpio = gpioMock.get();

		Servo uut(gpio);

		uut.SetPosition(100);

		fakeit::Verify(Method(gpioMock, GpioServo).Matching([](auto gpio, auto pulsewidth)
		{
			//Ignore the gpio, since it could change and is not important
			return pulsewidth == 2500;
		})); //It should call it once in the constructor and once again when we set it.

	}

	BOOST_AUTO_TEST_CASE(Servo_test_SetPosition_0_returns_500)
	{
		fakeit::Mock<IGPIO> gpioMock;
		fakeit::Fake(Method(gpioMock, GpioServo));
		fakeit::Fake(Method(gpioMock, GpioSetMode));

		IGPIO & gpio = gpioMock.get();

		Servo uut(gpio);

		uut.SetPosition(0);

		fakeit::Verify(Method(gpioMock, GpioServo).Matching([](auto gpio, auto pulsewidth)
		{
			//Ignore the gpio, since it could change and is not important
			return pulsewidth == 500;
		})); //It should call it once in the constructor and once again when we set it.

	}

	BOOST_AUTO_TEST_CASE(Servo_tests_get_status_after_constuctor)
	{
		fakeit::Mock<IGPIO> gpioMock;
		fakeit::Fake(Method(gpioMock, GpioServo));
		fakeit::Fake(Method(gpioMock, GpioSetMode));

		IGPIO & gpio = gpioMock.get();

		Servo uut(gpio);

		MotorStatus expected = MotorStatus(50, POSITION);
		auto result = uut.GetStatus();

		BOOST_REQUIRE(result.GetString() == expected.GetString());
	}

	BOOST_AUTO_TEST_CASE(Servo_tests_get_status_position_0)
	{
		fakeit::Mock<IGPIO> gpioMock;
		fakeit::Fake(Method(gpioMock, GpioServo));
		fakeit::Fake(Method(gpioMock, GpioSetMode));

		IGPIO & gpio = gpioMock.get();

		Servo uut(gpio);
		uut.SetPosition(0);

		MotorStatus expected = MotorStatus(0, POSITION);
		auto result = uut.GetStatus();

		BOOST_REQUIRE(result.GetString() == expected.GetString());
	}

	BOOST_AUTO_TEST_CASE(Servo_tests_get_status_position_100)
	{
		fakeit::Mock<IGPIO> gpioMock;
		fakeit::Fake(Method(gpioMock, GpioServo));
		fakeit::Fake(Method(gpioMock, GpioSetMode));

		IGPIO & gpio = gpioMock.get();

		Servo uut(gpio);
		uut.SetPosition(100);

		MotorStatus expected = MotorStatus(100, POSITION);
		auto result = uut.GetStatus();

		BOOST_REQUIRE(result.GetString() == expected.GetString());
	}
	

BOOST_AUTO_TEST_SUITE_END()