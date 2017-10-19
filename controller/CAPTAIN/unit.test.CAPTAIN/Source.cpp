#define BOOST_TEST_MODULE CaptainTests
#include <boost/test/included/unit_test.hpp> //It is slow because this is a huge header
#include <string>
#include "boost/fakeit.hpp"
#include "Coordinate.h"
#include "Pose.h"
#include "NavigationData.h"
#include "TargetPosition.h"
#include "CoverageRectangle.h"
#include "TaskData.h"
#include "DCMotor.h"
#include "Servo.h"
#include "MotorStatus.h"
#include "GPSStatus.h"

//#include <turtle/mock.hpp>

struct SomeInterface
{
	virtual int foo(int) = 0;
};

BOOST_AUTO_TEST_SUITE(All_tests)

	BOOST_AUTO_TEST_CASE(fakeit_test)
{
	fakeit::Mock<SomeInterface> mock;
	fakeit::When(Method(mock, foo)).Return(1);

	SomeInterface &i = mock.get();
	BOOST_REQUIRE(i.foo(10) == 1);
}


	BOOST_AUTO_TEST_SUITE(GPSStatus_tests)
		BOOST_AUTO_TEST_CASE(GPSStatus_GetString_returnsJSON)
		{
			GPSStatus uut = GPSStatus(50, 20, Pose(Coordinate(56.2, 10.8), 310)); //unit under test
			std::cout << uut.GetString() << std::endl;
			std::string compare = "{\"items_\":[{\"data_\":50.0,\"title_\":\"GPS frequency\",\"unit_\":\"Hz\"},{\"data_\":20.0,\"title_\":\"GPS delay\",\"unit_\":\"ms\"}],\"title_\":\"GPS Connection\"},{\"items_\":[{\"data_\":56.2,\"title_\":\"Latitude\",\"unit_\":\"deg\"},{\"data_\":10.8,\"title_\":\"Latitude\",\"unit_\":\"deg\"},{\"data_\":310.0,\"title_\":\"Orientation\",\"unit_\":\"deg\"}],\"title_\":\"Pose\"}";
			std::cout << compare << std::endl;

			BOOST_REQUIRE(uut.GetString() == compare);
		}
	BOOST_AUTO_TEST_SUITE_END()
	
	BOOST_AUTO_TEST_SUITE(TaskData_tests)
		BOOST_AUTO_TEST_CASE(TargetPosition_get_target_position_test)
		{
			TargetPosition uut = TargetPosition(Coordinate(3, 2));
			BOOST_REQUIRE(uut.get_target_postion().Longitude_ == 2 && uut.get_target_postion().Latitude_==3);
		}

		BOOST_AUTO_TEST_CASE(CoverageRectangle_get_start_position_test)
		{
			CoverageRectangle uut = CoverageRectangle(Coordinate(3, 2), Coordinate(1, 4));
			BOOST_REQUIRE(uut.get_start_position().Longitude_ == 2 && uut.get_start_position().Latitude_ == 3);
		}

		BOOST_AUTO_TEST_CASE(CoverageRectangle_get_end_position_test)
		{
			CoverageRectangle uut = CoverageRectangle(Coordinate(3, 2), Coordinate(1, 4));
			BOOST_REQUIRE(uut.get_end_position().Longitude_ == 4 && uut.get_end_position().Latitude_ == 1);
		}

		BOOST_AUTO_TEST_CASE(TaskData_CoverageRectangle_and_TargetPosition_in_vector)
		{
			std::vector<std::unique_ptr<TaskData>> tasks;
			tasks.push_back(std::make_unique<CoverageRectangle>(Coordinate(0, 0), Coordinate(1, 1)));
			tasks.push_back(std::make_unique<TargetPosition>(Coordinate(2, 2)));

			CoverageRectangle* cc = static_cast<CoverageRectangle*>(tasks[0].get());
			TargetPosition* tc = static_cast<TargetPosition*>(tasks[1].get());
			BOOST_REQUIRE(cc->get_start_position().Latitude_ == 0 && tc->get_target_postion().Latitude_==2);
		}

	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_SUITE(Coordinate_tests)
		BOOST_AUTO_TEST_CASE(Coordinate_constructor_latitude_test)
		{
			Coordinate uut = Coordinate(100, 200);
			BOOST_REQUIRE(uut.Latitude_ == 100);
		}

		BOOST_AUTO_TEST_CASE(Coordinate_constructor_longitude_test)
		{
			Coordinate uut = Coordinate(100, 200);
			BOOST_REQUIRE(uut.Longitude_ == 200);
		}

	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_SUITE(Pose_tests)
		
		BOOST_AUTO_TEST_CASE(Pose_constructor_coordinate_test)
		{
			Coordinate param1 = Coordinate(1, 2);
			Pose uut = Pose(param1, 0);

			BOOST_REQUIRE(uut.Coordinate_.Longitude_ == param1.Longitude_ && uut.Coordinate_.Latitude_ == param1.Latitude_);
		}

		BOOST_AUTO_TEST_CASE(Pose_constructor_orientation_test)
		{
			Coordinate param1 = Coordinate(0, 0);
			Pose uut = Pose(param1, 10);

			BOOST_REQUIRE(uut.Orientation_ == 10);
		}
	
	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_SUITE(NavigationData_tests)
		BOOST_AUTO_TEST_CASE(NavigationData_construcor_line_test)
		{
			Coordinate c1 = Coordinate(1, 1);
			Coordinate c2 = Coordinate(2, 2);
			Coordinate c3 = Coordinate(3, 3);
			std::vector<Coordinate> param1 = { c1,c2,c3 };
			NavigationData uut = NavigationData(param1);

			BOOST_REQUIRE(uut.Line_[0].Longitude_ == 1 && uut.Line_[1].Longitude_ == 2 && uut.Line_[2].Longitude_ == 3);

		}
		BOOST_AUTO_TEST_CASE(NavigationData_constructor_ete)
		{
			Coordinate c1 = Coordinate(1, 1);
			Coordinate c2 = Coordinate(2, 2);
			Coordinate c3 = Coordinate(3, 3);
			std::vector<Coordinate> param1 = { c1,c2,c3 };
			NavigationData uut = NavigationData(param1);

			BOOST_REQUIRE(uut.Ete_ == 0);
		}
	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_SUITE(DCMotor_tests)
		BOOST_AUTO_TEST_CASE(DCMotor_constructor_test)
	{
		DCMotor DCM1;
		BOOST_REQUIRE(DCM1.speed_ == 0.0);
	}

	BOOST_AUTO_TEST_CASE(DCMotor_SetSpeed_test)
	{
		DCMotor DCM2;
		DCM2.SetSpeed(5.6);
		BOOST_REQUIRE(DCM2.speed_ == 5.6);
	}

	BOOST_AUTO_TEST_CASE(DCMotor_GetStatus_percentage_test)
	{
		DCMotor DCM3;
		DCM3.SetSpeed(6.9);
		MotorStatus motorstatus1 = DCM3.GetStatus();
		BOOST_REQUIRE(motorstatus1.percentage_ == 6.9);
	}

	BOOST_AUTO_TEST_CASE(DCMotor_GetStatus_motor_type__test)
	{
		DCMotor DCM4;
		MotorStatus motorstatus2 = DCM4.GetStatus();
		BOOST_REQUIRE(motorstatus2.motor_type_ == SPEED);
	}

	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_SUITE(Servo_tests)
		BOOST_AUTO_TEST_CASE(Servo_constructor_test)
	{
		Servo Ser1;
		BOOST_REQUIRE(Ser1.position_ == 0.0);
	}

	BOOST_AUTO_TEST_CASE(Servo_SetPosition_test)
	{
		Servo Ser2;
		Ser2.SetPosition(7.9);
		BOOST_REQUIRE(Ser2.position_ == 7.9);
	}

	BOOST_AUTO_TEST_CASE(Servo_GetStatus_percentage_test)
	{
		Servo Ser3;
		Ser3.SetPosition(8.3);
		MotorStatus motorstatus3 = Ser3.GetStatus();
		BOOST_REQUIRE(motorstatus3.percentage_ == 8.3);
	}

	BOOST_AUTO_TEST_CASE(Servo_GetStatus_motor_type__test)
	{
		Servo Ser4;
		MotorStatus motorstatus4 = Ser4.GetStatus();
		BOOST_REQUIRE(motorstatus4.motor_type_ == POSITION);
	}

	BOOST_AUTO_TEST_SUITE_END()

		BOOST_AUTO_TEST_SUITE(MotorStatus_tests)
		BOOST_AUTO_TEST_CASE(MotorStatus_constructor_test)
	{
		DCMotor DCM5; //Test of this module only makes sense in the presence of a motor objekt!
		MotorStatus MS1 = DCM5.GetStatus();
		BOOST_REQUIRE(MS1.percentage_ == 0.0 && MS1.motor_type_ == 0);
	}

		BOOST_AUTO_TEST_CASE(MotorStatus_GetString_test)
	{
		DCMotor DCM6; //Test of this module only makes sense in the presence of a motor objekt!
		MotorStatus MS2 = DCM6.GetStatus();
		std::string MS_string = MS2.GetString();

		std::string test_string(R"({"items_":[{"color_","progress-bar-warning","data_":0.0,"title_":DC Motor,"unit_":"%"}],"title_":"Motor")");
		BOOST_REQUIRE(MS_string.compare(test_string) == 1);
	}

	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_SUITE_END()