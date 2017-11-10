#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "CAPTAIN/TargetPosition.h"
#include "CAPTAIN/CoverageRectangle.h"

BOOST_AUTO_TEST_SUITE(TaskData_tests)
	BOOST_AUTO_TEST_CASE(TargetPosition_get_target_position_test)
	{
		TargetPosition uut = TargetPosition(Coordinate(3, 2));
		BOOST_REQUIRE(uut.get_target_postion().Longitude_ == 2 && uut.get_target_postion().Latitude_ == 3);
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

	/*BOOST_AUTO_TEST_CASE(TaskData_CoverageRectangle_and_TargetPosition_in_vector)
{
std::vector<std::unique_ptr<TaskData>> tasks;
tasks.push_back(std::make_unique<CoverageRectangle>(Coordinate(0, 0), Coordinate(1, 1)));
tasks.push_back(std::make_unique<TargetPosition>(Coordinate(2, 2)));

CoverageRectangle* cc = static_cast<CoverageRectangle*>(tasks[0].get());
TargetPosition* tc = static_cast<TargetPosition*>(tasks[1].get());
BOOST_REQUIRE(cc->get_start_position().Latitude_ == 0 && tc->get_target_postion().Latitude_==2);
}*/

BOOST_AUTO_TEST_SUITE_END()