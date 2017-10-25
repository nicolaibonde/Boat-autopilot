#define BOOST_TEST_MODULE CaptainTests
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp> //It is slow because this is a huge header
#include <string>
#include "boost/fakeit.hpp"
#include "Coordinate.h"
#include "Pose.h"
#include "NavigationData.h"
#include "TargetPosition.h"
#include "CoverageRectangle.h"
#include "DCMotor.h"
#include "Servo.h"
#include "MotorStatus.h"
#include "GPSStatus.h"
#include "INavigation.h"
#include "IAutopilot.h"
#include "JSONReceiver.h"
#include "boost/fakeit.hpp"

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

			boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcP2P_malform_target_position");
			JSONReceiver uut(navigation, autopilot, path.string());

			uut.ReceiveToNav();

			//Expect None for the task and any thing in taskData and only one call

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)).Matching([](Task task, TargetPosition target_position)
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

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)).Matching([](Task task, TargetPosition target_position)
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

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)).Matching([](Task task, TargetPosition target_position)
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

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)).Matching([](Task task, TargetPosition target_position)
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

			boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcP2P_longitude_not_float");
			JSONReceiver uut(navigation, autopilot, path.string());

			uut.ReceiveToNav();

			//Expect None for the task and any thing in taskData and only one call

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, TargetPosition)).Matching([](Task task, TargetPosition target_position)
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
			fakeit::Fake(OverloadedMethod(navMock, PerformTask,void(Task,TargetPosition)));

			INavigation & navigation = navMock.get();

			fakeit::Mock<IAutopilot> autoMock;
			IAutopilot & autopilot = autoMock.get();

			boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcP2P");
			JSONReceiver uut(navigation, autopilot, path.string());

			uut.ReceiveToNav();

			 //Expect None for the task and any thing in taskData and only one call

			fakeit::Verify(OverloadedMethod(navMock, PerformTask,void(Task,TargetPosition)).Matching([](Task task, TargetPosition target_position)
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

			boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcCoverage_malform_start_coord");
			JSONReceiver uut(navigation, autopilot, path.string());
			uut.ReceiveToNav();
			//Expect None for the task and any thing in taskData and only one call

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task, CoverageRectangle coverage_rectangle)
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

			boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcCoverage_malform_start_coord_latitude");
			JSONReceiver uut(navigation, autopilot, path.string());
			uut.ReceiveToNav();
			//Expect None for the task and any thing in taskData and only one call

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task, CoverageRectangle coverage_rectangle)
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

			boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcCoverage_malform_start_coord_longitude");
			JSONReceiver uut(navigation, autopilot, path.string());
			uut.ReceiveToNav();
			//Expect None for the task and any thing in taskData and only one call

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task, CoverageRectangle coverage_rectangle)
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

			boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcCoverage_start_coord_latitude_not_float");
			JSONReceiver uut(navigation, autopilot, path.string());
			uut.ReceiveToNav();
			//Expect None for the task and any thing in taskData and only one call

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task, CoverageRectangle coverage_rectangle)
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

			boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcCoverage_start_coord_longitude_not_float");
			JSONReceiver uut(navigation, autopilot, path.string());
			uut.ReceiveToNav();
			//Expect None for the task and any thing in taskData and only one call

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task, CoverageRectangle coverage_rectangle)
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

			boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcCoverage_malform_end_coord");
			JSONReceiver uut(navigation, autopilot, path.string());
			uut.ReceiveToNav();
			//Expect None for the task and any thing in taskData and only one call

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task, CoverageRectangle coverage_rectangle)
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

			boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcCoverage_malform_end_coord_latitude");
			JSONReceiver uut(navigation, autopilot, path.string());
			uut.ReceiveToNav();
			//Expect None for the task and any thing in taskData and only one call

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task, CoverageRectangle coverage_rectangle)
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

			boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcCoverage_malform_end_coord_longitude");
			JSONReceiver uut(navigation, autopilot, path.string());
			uut.ReceiveToNav();
			//Expect None for the task and any thing in taskData and only one call

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task, CoverageRectangle coverage_rectangle)
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

			boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcCoverage_end_coord_latitude_not_float");
			JSONReceiver uut(navigation, autopilot, path.string());
			uut.ReceiveToNav();
			//Expect None for the task and any thing in taskData and only one call

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task, CoverageRectangle coverage_rectangle)
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

			boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/toNav/calcCoverage_end_coord_longitude_not_float");
			JSONReceiver uut(navigation, autopilot, path.string());
			uut.ReceiveToNav();
			//Expect None for the task and any thing in taskData and only one call

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task, CoverageRectangle coverage_rectangle)
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

			fakeit::Verify(OverloadedMethod(navMock, PerformTask, void(Task, CoverageRectangle)).Matching([](Task task, CoverageRectangle coverage_rectangle)
			{
				CoverageRectangle expected_coverage_rectangle(Coordinate(55.0,9.0), Coordinate(56.0, 10.0));
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

			fakeit::Verify(Method(navMock,SetParameters).Matching([](double tool_width )
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

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/malformParameter_names_");
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

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/parameter_names_item_not_string");
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

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/parameter_names_not_list");
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

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/parameters_and_parameter_names_not_same_length_");
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

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/parameters_item_not_float");
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

		boost::filesystem::path path = boost::filesystem::current_path().append("testJSON/activeProfile/parameters_not_list");
		JSONReceiver uut(navigation, autopilot, path.string());
		uut.ReceiveActiveParam();

		fakeit::Verify(Method(autoMock, SetParameters)).Never();
		fakeit::Verify(Method(navMock, SetParameters)).Never();
	}


	BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(GPSStatus_tests)
BOOST_AUTO_TEST_CASE(GPSStatus_GetString_returnsJSON)
{
	GPSStatus uut = GPSStatus(50, 20, Pose(Coordinate(56.2, 10.8), 310)); //unit under test
	//std::cout << uut.GetString() << std::endl;
	std::string compareString = (R"(
	{"items_":[
		{"data_":50.0,"title_":"GPS frequency","unit_":"Hz"},
		{"data_":20.0,"title_":"GPS delay","unit_":"ms"}],
		"title_":"GPS Connection"},
	{"items_":[
		{"data_":56.2,"title_":"Latitude","unit_":"deg"},
		{"data_":10.8,"title_":"Latitude","unit_":"deg"},
		{"data_":310.0,"title_":"Orientation","unit_":"deg"}],
		"title_":"Pose"}";
	)");
	//std::cout << compareString << std::endl;

	BOOST_REQUIRE(uut.GetString().compare(compareString) == 1);
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
	BOOST_AUTO_TEST_CASE(DCMotor_output_test)
	{

	}

	BOOST_AUTO_TEST_CASE(DCMotor_output2_test)
	{

	}

	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_SUITE(Servo_tests)
	BOOST_AUTO_TEST_CASE(Servo_output_test)
	{

	}

	BOOST_AUTO_TEST_CASE(Servo_output2_test)
	{

	}

	BOOST_AUTO_TEST_SUITE_END()

		BOOST_AUTO_TEST_SUITE(MotorStatus_tests)
		BOOST_AUTO_TEST_CASE(MotorStatus_GetString_test)
	{
		DCMotor DCM4; //Test of this module only makes sense in the presence of a motor objekt!
		MotorStatus MS2 = DCM4.GetStatus();
		std::string MS_string = MS2.GetString();

		std::string test_string(R"(

{"items_":[
	{
		"color_","progress-bar-warning",
		"data_":0.0,
		"title_":DC Motor,
		"unit_":"%"
	}
]
,"title_":"Motor"

)");
		BOOST_REQUIRE(MS_string.compare(test_string) == 1);
	}

	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_SUITE_END()
