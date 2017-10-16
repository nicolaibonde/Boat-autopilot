#define BOOST_TEST_MODULE CaptainTests
#include <boost/test/included/unit_test.hpp>
#include "Coordinate.h"
#include "Pose.h"
#include "NavigationData.h"


BOOST_AUTO_TEST_SUITE(All_tests)

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

	BOOST_AUTO_TEST_SUITE_END()