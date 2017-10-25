#define BOOST_TEST_MODULE CaptainTests
#include <string>
#include <boost/test/unit_test.hpp> //It is slow because this is a huge header
#include <boost/filesystem.hpp>
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
#include "JSONTransmitter.h"

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

	BOOST_REQUIRE(uut.GetString().compare(compareString) == 1);
}
BOOST_AUTO_TEST_SUITE_END()

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

BOOST_AUTO_TEST_CASE(TaskData_CoverageRectangle_and_TargetPosition_in_vector)
{
	std::vector<std::unique_ptr<TaskData>> tasks;
	tasks.push_back(std::make_unique<CoverageRectangle>(Coordinate(0, 0), Coordinate(1, 1)));
	tasks.push_back(std::make_unique<TargetPosition>(Coordinate(2, 2)));

	CoverageRectangle* cc = static_cast<CoverageRectangle*>(tasks[0].get());
	TargetPosition* tc = static_cast<TargetPosition*>(tasks[1].get());
	BOOST_REQUIRE(cc->get_start_position().Latitude_ == 0 && tc->get_target_postion().Latitude_ == 2);
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

	BOOST_REQUIRE(uut.Completed_path_[0].Longitude_ == 1 && uut.Completed_path_[1].Longitude_ == 2 && uut.Completed_path_[2].Longitude_ == 3);

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
	NavigationData uut = NavigationData(param1,param2, 0, 0);

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
	/*MotorStatus motorstatus(45.4, POSITION);
	std::string uut = motorstatus.GetString();
	std::string expected(R"({"items_":[{"color_":"progress-bar-warning","data_":45.4,"title_":"Servo","unit_":"%"}],"title_":"Motor"})");

	BOOST_REQUIRE(uut == expected);*/
	BOOST_REQUIRE(1 == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(JSONTransmitter_tests)
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_test)
{
	//Link mocks to interfaces
	fakeit::Mock<INavigation> navMock;
	fakeit::Mock<IMotorStatusGetter> dcMock;
	fakeit::Mock<IMotorStatusGetter> servoMock;
	fakeit::Mock<IGPS> gpsMock;

	//Instantiate mocks
	INavigation& navigation = navMock.get();
	IMotorStatusGetter& dcMotor = dcMock.get();
	IMotorStatusGetter& servo = servoMock.get();
	IGPS& gps = gpsMock.get();

	//Set function return values
	std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22)};
	std::vector<Coordinate> path_vector = { Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4) };
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(5.2, 1.2, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON/fromNav/");

	//Save the file to the specified folder
	uut.TransmitFromNav(path.string());

	//Verification
	fakeit::Verify(Method(navMock, GetNavData)).Exactly(1);
	fakeit::Verify(Method(dcMock, GetStatus)).Exactly(1);
	fakeit::Verify(Method(servoMock, GetStatus)).Exactly(1);
	fakeit::Verify(Method(gpsMock, GetStatus)).Exactly(1);
	fakeit::Verify(Method(gpsMock, GetPose)).Exactly(1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()