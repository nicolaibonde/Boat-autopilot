#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "CAPTAIN/DCMotor.h"
#include "boost/fakeit.hpp"

BOOST_AUTO_TEST_SUITE(DCMotor_tests)

	BOOST_AUTO_TEST_CASE(DCMotor_test_constructor)
	{
		fakeit::Mock<IGPIO> gpioMock;

		IGPIO& gpio = gpioMock.get();

		fakeit::Fake(Method(gpioMock, GpioSetMode));
		fakeit::Fake(Method(gpioMock, GpioHardwarePWM));

		DCMotor uut(gpio);

		fakeit::Verify(Method(gpioMock, GpioSetMode)).Exactly(fakeit::Once);
		fakeit::Verify(Method(gpioMock, GpioHardwarePWM)).Exactly(fakeit::Once);
		//I dont think it make sense to check on what it is set to since that is hardware specific
	}

	BOOST_AUTO_TEST_CASE(DCMotor_test_constructor_pwm_0)
	{
		fakeit::Mock<IGPIO> gpioMock;

		IGPIO& gpio = gpioMock.get();

		fakeit::Fake(Method(gpioMock, GpioSetMode));
		fakeit::Fake(Method(gpioMock, GpioHardwarePWM));

		DCMotor uut(gpio);

		fakeit::Verify(Method(gpioMock, GpioHardwarePWM).Matching([](auto gpio_pin, auto frequency, auto pulsewidth)
		{
			return pulsewidth == 0;
		})).Once();
	}

	BOOST_AUTO_TEST_CASE(DCMotor_test_SetSpeed_50_result_500000)
	{
		fakeit::Mock<IGPIO> gpioMock;

		IGPIO& gpio = gpioMock.get();

		fakeit::Fake(Method(gpioMock, GpioSetMode));
		fakeit::Fake(Method(gpioMock, GpioHardwarePWM));

		DCMotor uut(gpio);

		uut.SetSpeed(50);

		fakeit::Verify(Method(gpioMock, GpioHardwarePWM).Matching([](auto gpio_pin, auto frequency, auto pulsewidth)
		{
			return pulsewidth == 50 * 10000;
		})).AtLeastOnce();
	}

	BOOST_AUTO_TEST_CASE(DCMotor_test_SetSpeed_negative_10_result_0)
	{
		fakeit::Mock<IGPIO> gpioMock;

		IGPIO& gpio = gpioMock.get();

		fakeit::Fake(Method(gpioMock, GpioSetMode));
		fakeit::Fake(Method(gpioMock, GpioHardwarePWM));

		DCMotor uut(gpio);

		uut.SetSpeed(-10);

		fakeit::Verify(Method(gpioMock, GpioHardwarePWM).Matching([](auto gpio_pin, auto frequency, auto pulsewidth)
		{
			return pulsewidth == 0;
		})).AtLeastOnce();
	}

	BOOST_AUTO_TEST_CASE(DCMotor_test_SetSpeed_110_result_1000000)
	{
		fakeit::Mock<IGPIO> gpioMock;

		IGPIO& gpio = gpioMock.get();

		fakeit::Fake(Method(gpioMock, GpioSetMode));
		fakeit::Fake(Method(gpioMock, GpioHardwarePWM));

		DCMotor uut(gpio);

		uut.SetSpeed(110);

		fakeit::Verify(Method(gpioMock, GpioHardwarePWM).Matching([](auto gpio_pin, auto frequency, auto pulsewidth)
		{
			return pulsewidth == 100 * 10000;
		})).AtLeastOnce();
	}

	BOOST_AUTO_TEST_CASE(DCMotor_test_SetSpeed_0_result_0)
	{
		fakeit::Mock<IGPIO> gpioMock;

		IGPIO& gpio = gpioMock.get();

		fakeit::Fake(Method(gpioMock, GpioSetMode));
		fakeit::Fake(Method(gpioMock, GpioHardwarePWM));

		DCMotor uut(gpio);

		uut.SetSpeed(0);

		fakeit::Verify(Method(gpioMock, GpioHardwarePWM).Matching([](auto gpio_pin, auto frequency, auto pulsewidth)
		{
			return pulsewidth == 0;
		})).AtLeastOnce();
	}

	BOOST_AUTO_TEST_CASE(DCMotor_test_SetSpeed_100_result_1000000)
	{
		fakeit::Mock<IGPIO> gpioMock;

		IGPIO& gpio = gpioMock.get();

		fakeit::Fake(Method(gpioMock, GpioSetMode));
		fakeit::Fake(Method(gpioMock, GpioHardwarePWM));

		DCMotor uut(gpio);

		uut.SetSpeed(100);

		fakeit::Verify(Method(gpioMock, GpioHardwarePWM).Matching([](auto gpio_pin,auto frequency, auto pulsewidth)
		{
			return pulsewidth == 100 * 10000;
		})).AtLeastOnce();
	}
BOOST_AUTO_TEST_SUITE_END()