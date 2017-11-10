#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "CAPTAIN/Coordinate.h"

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