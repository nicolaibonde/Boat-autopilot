#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "boost/fakeit.hpp"
#include "CAPTAIN/INavigation.h"
#include "CAPTAIN/IAutopilot.h"
#include <boost/filesystem.hpp>
#include "CAPTAIN/JSONReceiver.h"

BOOST_AUTO_TEST_SUITE(JSONReceiver_tests)

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_unknown_func_)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/unknown");
		JSONReceiver uut(navigation, autopilot, path.string());

		uut.ReceiveToNav();

		Task t = None; //Expect None for the task and any thing in taskData and only one call
		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task)).Using(t)).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_empty_toNav_file)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();
		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/empty");
		JSONReceiver uut(navigation, autopilot, path.string());

		uut.ReceiveToNav();

		Task t = None; //Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task)).Using(t)).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_none)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();
		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/none");
		JSONReceiver uut(navigation, autopilot, path.string());

		uut.ReceiveToNav();

		Task t = None; //Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task)).Using(t)).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_start)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();
		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/start");
		JSONReceiver uut(navigation, autopilot, path.string());

		uut.ReceiveToNav();

		Task t = Start; //Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task)).Using(t)).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_stop)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();
		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/stop");
		JSONReceiver uut(navigation, autopilot, path.string());

		uut.ReceiveToNav();

		Task t = Stop; //Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task)).Using(t)).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcP2P_malform_target_position_)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/toNav/calcP2P_malform_target_position");
		JSONReceiver uut(navigation, autopilot, path.string());

		uut.ReceiveToNav();

		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)).Matching([](Task task,
			TargetPosition target_position)
		{
			TargetPosition expected_target_position = (Coordinate(-1, -1));
			Task expected_task = CalcP2P;

			return
			expected_task == task &&
			expected_target_position.get_target_postion().Latitude_ == target_position.get_target_postion().Latitude_ &&
			expected_target_position.get_target_postion().Longitude_ == target_position.get_target_postion().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcP2P_malform_latitude_)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcP2P_malform_latitude");
		JSONReceiver uut(navigation, autopilot, path.string());

		uut.ReceiveToNav();

		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)).Matching([](Task task,
			TargetPosition target_position)
		{
			TargetPosition expected_target_position = (Coordinate(-1, -1));
			Task expected_task = CalcP2P;

			return
			expected_task == task &&
			expected_target_position.get_target_postion().Latitude_ == target_position.get_target_postion().Latitude_ &&
			expected_target_position.get_target_postion().Longitude_ == target_position.get_target_postion().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcP2P_malform_longitude_)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcP2P_malform_longitude");
		JSONReceiver uut(navigation, autopilot, path.string());

		uut.ReceiveToNav();

		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)).Matching([](Task task,
			TargetPosition target_position)
		{
			TargetPosition expected_target_position = (Coordinate(-1, -1));
			Task expected_task = CalcP2P;

			return
			expected_task == task &&
			expected_target_position.get_target_postion().Latitude_ == target_position.get_target_postion().Latitude_ &&
			expected_target_position.get_target_postion().Longitude_ == target_position.get_target_postion().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcP2P_latitude_not_float)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcP2P_latitude_not_float");
		JSONReceiver uut(navigation, autopilot, path.string());

		uut.ReceiveToNav();

		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)).Matching([](Task task,
			TargetPosition target_position)
		{
			TargetPosition expected_target_position = (Coordinate(-1, -1));
			Task expected_task = CalcP2P;

			return
			expected_task == task &&
			expected_target_position.get_target_postion().Latitude_ == target_position.get_target_postion().Latitude_ &&
			expected_target_position.get_target_postion().Longitude_ == target_position.get_target_postion().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcP2P_longitude_not_float)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().
			append("testJSON/toNav/calcP2P_longitude_not_float");
		JSONReceiver uut(navigation, autopilot, path.string());

		uut.ReceiveToNav();

		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)).Matching([](Task task,
			TargetPosition target_position)
		{
			TargetPosition expected_target_position = (Coordinate(-1, -1));
			Task expected_task = CalcP2P;

			return
			expected_task == task &&
			expected_target_position.get_target_postion().Latitude_ == target_position.get_target_postion().Latitude_ &&
			expected_target_position.get_target_postion().Longitude_ == target_position.get_target_postion().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcP2P_latitude_56_longitude_10_)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcP2P");
		JSONReceiver uut(navigation, autopilot, path.string());

		uut.ReceiveToNav();

		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)).Matching([](Task task,
			TargetPosition target_position)
		{
			TargetPosition expected_target_position = (Coordinate(56.0, 10.0));
			Task expected_task = CalcP2P;

			return
			expected_task == task &&
			expected_target_position.get_target_postion().Latitude_ == target_position.get_target_postion().Latitude_ &&
			expected_target_position.get_target_postion().Longitude_ == target_position.get_target_postion().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcCoverage_malform_start_coord_)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/toNav/calcCoverage_malform_start_coord");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveToNav();
		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task,
			CoverageRectangle coverage_rectangle)
		{
			CoverageRectangle expected_coverage_rectangle(Coordinate(-1, -1), Coordinate(-1, -1));
			Task expected_task = CalcCoverageRectangle;

			return
			expected_task == task &&
			expected_coverage_rectangle.get_start_position().Latitude_ == coverage_rectangle.get_start_position().Latitude_ &&
			expected_coverage_rectangle.get_start_position().Longitude_ == coverage_rectangle.get_start_position().Longitude_ &&
			expected_coverage_rectangle.get_end_position().Latitude_ == coverage_rectangle.get_end_position().Latitude_ &&
			expected_coverage_rectangle.get_end_position().Longitude_ == coverage_rectangle.get_end_position().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcCoverage_malform_start_coord_latitude)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/toNav/calcCoverage_malform_start_coord_latitude");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveToNav();
		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task,
			CoverageRectangle coverage_rectangle)
		{
			CoverageRectangle expected_coverage_rectangle(Coordinate(-1, -1), Coordinate(-1, -1));
			Task expected_task = CalcCoverageRectangle;

			return
			expected_task == task &&
			expected_coverage_rectangle.get_start_position().Latitude_ == coverage_rectangle.get_start_position().Latitude_ &&
			expected_coverage_rectangle.get_start_position().Longitude_ == coverage_rectangle.get_start_position().Longitude_ &&
			expected_coverage_rectangle.get_end_position().Latitude_ == coverage_rectangle.get_end_position().Latitude_ &&
			expected_coverage_rectangle.get_end_position().Longitude_ == coverage_rectangle.get_end_position().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcCoverage_malform_start_coord_longitude)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/toNav/calcCoverage_malform_start_coord_longitude");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveToNav();
		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task,
			CoverageRectangle coverage_rectangle)
		{
			CoverageRectangle expected_coverage_rectangle(Coordinate(-1, -1), Coordinate(-1, -1));
			Task expected_task = CalcCoverageRectangle;

			return
			expected_task == task &&
			expected_coverage_rectangle.get_start_position().Latitude_ == coverage_rectangle.get_start_position().Latitude_ &&
			expected_coverage_rectangle.get_start_position().Longitude_ == coverage_rectangle.get_start_position().Longitude_ &&
			expected_coverage_rectangle.get_end_position().Latitude_ == coverage_rectangle.get_end_position().Latitude_ &&
			expected_coverage_rectangle.get_end_position().Longitude_ == coverage_rectangle.get_end_position().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcCoverage_start_coord_latitude_not_float)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/toNav/calcCoverage_start_coord_latitude_not_float");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveToNav();
		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task,
			CoverageRectangle coverage_rectangle)
		{
			CoverageRectangle expected_coverage_rectangle(Coordinate(-1, -1), Coordinate(-1, -1));
			Task expected_task = CalcCoverageRectangle;

			return
			expected_task == task &&
			expected_coverage_rectangle.get_start_position().Latitude_ == coverage_rectangle.get_start_position().Latitude_ &&
			expected_coverage_rectangle.get_start_position().Longitude_ == coverage_rectangle.get_start_position().Longitude_ &&
			expected_coverage_rectangle.get_end_position().Latitude_ == coverage_rectangle.get_end_position().Latitude_ &&
			expected_coverage_rectangle.get_end_position().Longitude_ == coverage_rectangle.get_end_position().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcCoverage_start_coord_longitude_not_float)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/toNav/calcCoverage_start_coord_longitude_not_float");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveToNav();
		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task,
			CoverageRectangle coverage_rectangle)
		{
			CoverageRectangle expected_coverage_rectangle(Coordinate(-1, -1), Coordinate(-1, -1));
			Task expected_task = CalcCoverageRectangle;

			return
			expected_task == task &&
			expected_coverage_rectangle.get_start_position().Latitude_ == coverage_rectangle.get_start_position().Latitude_ &&
			expected_coverage_rectangle.get_start_position().Longitude_ == coverage_rectangle.get_start_position().Longitude_ &&
			expected_coverage_rectangle.get_end_position().Latitude_ == coverage_rectangle.get_end_position().Latitude_ &&
			expected_coverage_rectangle.get_end_position().Longitude_ == coverage_rectangle.get_end_position().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcCoverage_malform_end_coord_)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/toNav/calcCoverage_malform_end_coord");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveToNav();
		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task,
			CoverageRectangle coverage_rectangle)
		{
			CoverageRectangle expected_coverage_rectangle(Coordinate(-1, -1), Coordinate(-1, -1));
			Task expected_task = CalcCoverageRectangle;

			return
			expected_task == task &&
			expected_coverage_rectangle.get_start_position().Latitude_ == coverage_rectangle.get_start_position().Latitude_ &&
			expected_coverage_rectangle.get_start_position().Longitude_ == coverage_rectangle.get_start_position().Longitude_ &&
			expected_coverage_rectangle.get_end_position().Latitude_ == coverage_rectangle.get_end_position().Latitude_ &&
			expected_coverage_rectangle.get_end_position().Longitude_ == coverage_rectangle.get_end_position().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcCoverage_malform_end_coord_latitude)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/toNav/calcCoverage_malform_end_coord_latitude");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveToNav();
		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task,
			CoverageRectangle coverage_rectangle)
		{
			CoverageRectangle expected_coverage_rectangle(Coordinate(-1, -1), Coordinate(-1, -1));
			Task expected_task = CalcCoverageRectangle;

			return
			expected_task == task &&
			expected_coverage_rectangle.get_start_position().Latitude_ == coverage_rectangle.get_start_position().Latitude_ &&
			expected_coverage_rectangle.get_start_position().Longitude_ == coverage_rectangle.get_start_position().Longitude_ &&
			expected_coverage_rectangle.get_end_position().Latitude_ == coverage_rectangle.get_end_position().Latitude_ &&
			expected_coverage_rectangle.get_end_position().Longitude_ == coverage_rectangle.get_end_position().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcCoverage_malform_end_coord_longitude)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/toNav/calcCoverage_malform_end_coord_longitude");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveToNav();
		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task,
			CoverageRectangle coverage_rectangle)
		{
			CoverageRectangle expected_coverage_rectangle(Coordinate(-1, -1), Coordinate(-1, -1));
			Task expected_task = CalcCoverageRectangle;

			return
			expected_task == task &&
			expected_coverage_rectangle.get_start_position().Latitude_ == coverage_rectangle.get_start_position().Latitude_ &&
			expected_coverage_rectangle.get_start_position().Longitude_ == coverage_rectangle.get_start_position().Longitude_ &&
			expected_coverage_rectangle.get_end_position().Latitude_ == coverage_rectangle.get_end_position().Latitude_ &&
			expected_coverage_rectangle.get_end_position().Longitude_ == coverage_rectangle.get_end_position().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcCoverage_end_coord_latitude_not_float)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/toNav/calcCoverage_end_coord_latitude_not_float");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveToNav();
		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task,
			CoverageRectangle coverage_rectangle)
		{
			CoverageRectangle expected_coverage_rectangle(Coordinate(-1, -1), Coordinate(-1, -1));
			Task expected_task = CalcCoverageRectangle;

			return
			expected_task == task &&
			expected_coverage_rectangle.get_start_position().Latitude_ == coverage_rectangle.get_start_position().Latitude_ &&
			expected_coverage_rectangle.get_start_position().Longitude_ == coverage_rectangle.get_start_position().Longitude_ &&
			expected_coverage_rectangle.get_end_position().Latitude_ == coverage_rectangle.get_end_position().Latitude_ &&
			expected_coverage_rectangle.get_end_position().Longitude_ == coverage_rectangle.get_end_position().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcCoverage_end_coord_longitude_not_float)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/toNav/calcCoverage_end_coord_longitude_not_float");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveToNav();
		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task,
			CoverageRectangle coverage_rectangle)
		{
			CoverageRectangle expected_coverage_rectangle(Coordinate(-1, -1), Coordinate(-1, -1));
			Task expected_task = CalcCoverageRectangle;

			return
			expected_task == task &&
			expected_coverage_rectangle.get_start_position().Latitude_ == coverage_rectangle.get_start_position().Latitude_ &&
			expected_coverage_rectangle.get_start_position().Longitude_ == coverage_rectangle.get_start_position().Longitude_ &&
			expected_coverage_rectangle.get_end_position().Latitude_ == coverage_rectangle.get_end_position().Latitude_ &&
			expected_coverage_rectangle.get_end_position().Longitude_ == coverage_rectangle.get_end_position().Longitude_;

		})).Exactly(1);
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveToNav_calcCoverage_)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)));

		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcCoverage");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveToNav();
		//Expect None for the task and any thing in taskData and only one call

		fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task,
			CoverageRectangle coverage_rectangle)
		{
			CoverageRectangle expected_coverage_rectangle(Coordinate(55.0, 9.0), Coordinate(56.0, 10.0));
			Task expected_task = CalcCoverageRectangle;

			return
			expected_task == task &&
			expected_coverage_rectangle.get_start_position().Latitude_ == coverage_rectangle.get_start_position().Latitude_ &&
			expected_coverage_rectangle.get_start_position().Longitude_ == coverage_rectangle.get_start_position().Longitude_ &&
			expected_coverage_rectangle.get_end_position().Latitude_ == coverage_rectangle.get_end_position().Latitude_ &&
			expected_coverage_rectangle.get_end_position().Longitude_ == coverage_rectangle.get_end_position().Longitude_;

		}));
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_SetParam_Nav)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/setParamNav");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(navMock, SetParameters).Matching([](double tool_width)
		{
			double expected = 4;
			return expected == tool_width;
		}));

	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_SetParam_autoPilot)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/setParamNav");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters).Matching([](double P, double I, double D)
		{
			double expected_P = 1;
			double expected_I = 2;
			double expected_D = 3;

			return	expected_P == P &&
			expected_I == I &&
			expected_D == D;
		}));

	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_empty)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/empty");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters)).Never();
		fakeit::Verify(Method(navMock, SetParameters)).Never();
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_malform_parameter_names_)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/activeProfile/malformParameter_names_");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters)).Never();
		fakeit::Verify(Method(navMock, SetParameters)).Never();
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_malform_parameters_)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/malformParameters_");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters)).Never();
		fakeit::Verify(Method(navMock, SetParameters)).Never();
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_missing_D)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/missingD");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters)).Never();
		fakeit::Verify(Method(navMock, SetParameters)).Never();
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_missing_P)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/missingP");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters)).Never();
		fakeit::Verify(Method(navMock, SetParameters)).Never();
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_missing_I)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/missingI");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters)).Never();
		fakeit::Verify(Method(navMock, SetParameters)).Never();
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_missing_tool_width)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/missingTool_width");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters)).Never();
		fakeit::Verify(Method(navMock, SetParameters)).Never();
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_parameter_names_item_not_string)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/activeProfile/parameter_names_item_not_string");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters)).Never();
		fakeit::Verify(Method(navMock, SetParameters)).Never();
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_parameter_names_not_list)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/activeProfile/parameter_names_not_list");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters)).Never();
		fakeit::Verify(Method(navMock, SetParameters)).Never();
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_parameter_and_parameter_names_not_same_length)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/activeProfile/parameters_and_parameter_names_not_same_length_");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters)).Never();
		fakeit::Verify(Method(navMock, SetParameters)).Never();
	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_parameters_item_not_float)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().append(
			"testJSON/activeProfile/parameters_item_not_float");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters)).Never();
		fakeit::Verify(Method(navMock, SetParameters)).Never();

	}

	BOOST_AUTO_TEST_CASE(JSONReceiver_ReceiveActiveProfile_parameters_not_list)
	{
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Method(navMock, SetParameters));
		INavigation & navigation = navMock.get();

		fakeit::Mock<IAutopilot> autoMock;
		fakeit::Fake(Method(autoMock, SetParameters));
		IAutopilot & autopilot = autoMock.get();

		boost::filesystem::path path = boost::filesystem::current_path().
			append("testJSON/activeProfile/parameters_not_list");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters)).Never();
		fakeit::Verify(Method(navMock, SetParameters)).Never();
	}


BOOST_AUTO_TEST_SUITE_END()