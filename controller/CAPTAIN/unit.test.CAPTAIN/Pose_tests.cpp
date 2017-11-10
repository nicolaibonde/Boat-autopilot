#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "CAPTAIN/Pose.h"


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