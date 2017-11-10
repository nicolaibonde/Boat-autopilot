#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "CAPTAIN/GPSStatus.h"

BOOST_AUTO_TEST_SUITE(GPSStatus_tests)
	BOOST_AUTO_TEST_CASE(GPSStatus_GetString_returnsJSON)
	{
		GPSStatus uut = GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.2, 10.8), 310)); //unit under test
		//std::cout << uut.GetString() << std::endl;
		std::string compareString = (
			R"(
	{"items_":[
		{"data_":1.1,"title_":"GPS fix","unit_":""},
		{"data_":2.2,"title_":"Satelites","unit_":""},
		{"data_":3.3,"title_":"Horizontal dilution of precision","unit_":""},
		{"data_":4.4,"title_":"Fix timestamp","unit_":"UTC"}],
		"title_":"GPS Connection"},
	{"items_":[
		{"data_":56.2,"title_":"Latitude","unit_":"deg"},
		{"data_":10.8,"title_":"Latitude","unit_":"deg"},
		{"data_":310.0,"title_":"Orientation","unit_":"deg"}],
		"title_":"Pose"}";
	)"
		);
		//std::cout << compareString << std::endl;

		BOOST_REQUIRE(uut.GetString().compare(compareString) == 1);
	}
BOOST_AUTO_TEST_SUITE_END()