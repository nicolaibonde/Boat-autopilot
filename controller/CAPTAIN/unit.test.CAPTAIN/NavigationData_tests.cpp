#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "CAPTAIN/Coordinate.h"
#include "CAPTAIN/NavigationData.h"

BOOST_AUTO_TEST_SUITE(NavigationData_tests)
	BOOST_AUTO_TEST_CASE(NavigationData_construcor_completed_path_test)
	{
		Coordinate c1 = Coordinate(1, 1);
		Coordinate c2 = Coordinate(2, 2);
		Coordinate c3 = Coordinate(3, 3);
		Coordinate c4 = Coordinate(4, 4);
		Coordinate c5 = Coordinate(5, 5);
		Coordinate c6 = Coordinate(6, 6);
		std::vector<Coordinate> param1 = { c1,c2,c3 };
		std::vector<Coordinate> param2 = { c4,c5,c6 };
		NavigationData uut = NavigationData(param1, param2, 0, 0);

		BOOST_REQUIRE(uut.Completed_path_[0].Longitude_ == 1 && uut.Completed_path_[1].Longitude_ == 2 && uut.Completed_path_
			[2].Longitude_ == 3);

	}

	BOOST_AUTO_TEST_CASE(NavigationData_construcor_line_test)
	{
		Coordinate c1 = Coordinate(1, 1);
		Coordinate c2 = Coordinate(2, 2);
		Coordinate c3 = Coordinate(3, 3);
		Coordinate c4 = Coordinate(4, 4);
		Coordinate c5 = Coordinate(5, 5);
		Coordinate c6 = Coordinate(6, 6);
		std::vector<Coordinate> param1 = { c1,c2,c3 };
		std::vector<Coordinate> param2 = { c4,c5,c6 };
		NavigationData uut = NavigationData(param1, param2, 0, 0);

		BOOST_REQUIRE(uut.Path_[0].Longitude_ == 4 && uut.Path_[1].Longitude_ == 5 && uut.Path_[2].Longitude_ == 6);

	}
	BOOST_AUTO_TEST_CASE(NavigationData_constructor_ete)
	{
		Coordinate c1 = Coordinate(1, 1);
		Coordinate c2 = Coordinate(2, 2);
		Coordinate c3 = Coordinate(3, 3);
		Coordinate c4 = Coordinate(4, 4);
		Coordinate c5 = Coordinate(5, 5);
		Coordinate c6 = Coordinate(6, 6);
		std::vector<Coordinate> param1 = { c1,c2,c3 };
		std::vector<Coordinate> param2 = { c4,c5,c6 };
		NavigationData uut = NavigationData(param1, param2, 123, 0);

		BOOST_REQUIRE(uut.Ete_ == 123);
	}
	BOOST_AUTO_TEST_CASE(NavigationData_constructor_progress)
	{
		Coordinate c1 = Coordinate(1, 1);
		Coordinate c2 = Coordinate(2, 2);
		Coordinate c3 = Coordinate(3, 3);
		Coordinate c4 = Coordinate(4, 4);
		Coordinate c5 = Coordinate(5, 5);
		Coordinate c6 = Coordinate(6, 6);
		std::vector<Coordinate> param1 = { c1,c2,c3 };
		std::vector<Coordinate> param2 = { c4,c5,c6 };
		NavigationData uut = NavigationData(param1, param2, 0, 5.18);

		BOOST_REQUIRE(uut.Progress_ == 5.18);
	}
BOOST_AUTO_TEST_SUITE_END()
