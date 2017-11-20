// test1.cpp
#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "boost/fakeit.hpp"
#include "CAPTAIN/INavigation.h"
#include "CAPTAIN/IMotorStatusGetter.h"
#include "CAPTAIN/IGPS.h"
#include "CAPTAIN/Pose.h"
#include "CAPTAIN/Coordinate.h"
#include "CAPTAIN/JSONTransmitter.h"
#include <boost/filesystem.hpp>
#include "CAPTAIN/json.hpp"

BOOST_AUTO_TEST_SUITE(JSONTransmitter_tests)

	//This test will verify that the first completed_path latitude input produces the expected output
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_completed_path_latitude_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected and result strings
		const std::string expected = "56.08";
		const std::string from_nav_result = from_nav_json.at("Completed_path_").at("line_").at(0).at("latitude_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}
	
	//This test will verify that the first completed_path longitude input produces the expected output
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_completed_path_longitude_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected and result strings
		const std::string expected = "10.12";
		const std::string from_nav_result = from_nav_json.at("Completed_path_").at("line_").at(0).at("longitude_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the second completed_path latitude input produces the expected output
	//and thus confirm that the vector iterator behaves as expected.
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_completed_path_next_latitude_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected and result strings
		const std::string expected = "56.18";
		const std::string from_nav_result = from_nav_json.at("Completed_path_").at("line_").at(1).at("latitude_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the second completed_path longitude input produces the expected output
	//and thus confirm that the vector iterator behaves as expected.
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_completed_path_next_longitude_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected and result strings
		const std::string expected = "10.22";
		const std::string from_nav_result = from_nav_json.at("Completed_path_").at("line_").at(1).at("longitude_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the first path latitude input produces the expected output
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_path_latitude_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected and result strings
		const std::string expected = "56.1";
		const std::string from_nav_result = from_nav_json.at("Path_").at("line_").at(0).at("latitude_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the first path longitude input produces the expected output
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_path_longitude_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected string
		//Create expected and result strings
		const std::string expected = "10.1";
		const std::string from_nav_result = from_nav_json.at("Path_").at("line_").at(0).at("longitude_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the second path latitude input produces the expected output
	//and thus confirm that the vector iterator behaves as expected.
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_path_next_latitude_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected and result strings
		const std::string expected = "56.2";
		const std::string from_nav_result = from_nav_json.at("Path_").at("line_").at(1).at("latitude_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the second path longitude input produces the expected output
	//and thus confirm that the vector iterator behaves as expected.
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_path_next_longitude_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected and result strings
		const std::string expected = "10.2";
		const std::string from_nav_result = from_nav_json.at("Path_").at("line_").at(1).at("longitude_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the Progress ete input produces the expected output
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Progress_ete_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected string
		const std::string expected = R"("2 min 31 sec")";
		const std::string from_nav_result = from_nav_json.at("Progress_").at("ete_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the Progress percentage input produces the expected output
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Progress_percentage_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected string
		const std::string expected = "60.9";
		const std::string from_nav_result = from_nav_json.at("Progress_").at("percentage_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the DCMotor percentage input produces the expected output
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Motor_DCMotorPercentage_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected string
		const std::string expected = "15.44";
		std::string from_nav_result;

		//Extract string from nested object
		for (nlohmann::json::iterator it = from_nav_json.at("Status_").begin(); it != from_nav_json.at("Status_").end(); ++it)
		{
			if (it.value().at("title_") == "Propulsion")
			{
				for (nlohmann::json::iterator it2 = from_nav_json.at("Status_").at(it - from_nav_json.at("Status_").begin()).at("items_").begin();
				     it2 != from_nav_json.at("Status_").at(it - from_nav_json.at("Status_").begin()).at("items_").end(); ++it2)
				{
					const double from_nav_result_double = it2.value().at("data_");
					std::ostringstream from_nav_result_stringstream;
					from_nav_result_stringstream << from_nav_result_double;
					from_nav_result = from_nav_result_stringstream.str();
				}
			}
		}

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the Servo percentage input produces the expected output
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Motor_ServoPercentage_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected string
		const std::string expected = "45.44";
		std::string from_nav_result;

		//Extract string from nested object
		for (nlohmann::json::iterator it = from_nav_json.at("Status_").begin(); it != from_nav_json.at("Status_").end(); ++it)
		{
			if (it.value().at("title_") == "Position")
			{
				for (nlohmann::json::iterator it2 = from_nav_json.at("Status_").at(it - from_nav_json.at("Status_").begin()).at("items_").begin();
				     it2 != from_nav_json.at("Status_").at(it - from_nav_json.at("Status_").begin()).at("items_").end(); ++it2)
				{
					const double from_nav_result_double = it2.value().at("data_");
					std::ostringstream from_nav_result_stringstream;
					from_nav_result_stringstream << from_nav_result_double;
					from_nav_result = from_nav_result_stringstream.str();
				}
			}
		}

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the telemetry (and pose) latitude input(s) produce(s) the expected output
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Telemetry_and_pose_latitude_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected string
		const std::string expected = "56.0";
		const std::string from_nav_result = from_nav_json.at("Telemetry_").at("latitude_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the telemetry (and pose) longitude input(s) produce(s) the expected output
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Telemetry_and_pose_longitude_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected string
		const std::string expected = "10.0";
		const std::string from_nav_result = from_nav_json.at("Telemetry_").at("longitude_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the telemetry (and pose) orientation input(s) produce(s) the expected output
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Telemetry_and_pose_orientation_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected string
		const std::string expected = "7.45";
		const std::string from_nav_result = from_nav_json.at("Telemetry_").at("orientation_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}

	//This test will verify that the timestamp input produces the expected output
	BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_timestamp_test)
	{
		//Arrange
		//Link mocks to interfaces, must stub Dtor of navMock to get an INavigation* later
		fakeit::Mock<INavigation> navMock;
		fakeit::Fake(Dtor(navMock));
		fakeit::Mock<IMotorStatusGetter> dcMock;
		fakeit::Mock<IMotorStatusGetter> servoMock;
		fakeit::Mock<IGPS> gpsMock;

		//Instantiate mocks
		INavigation* nav_ptr = &navMock.get();
		IMotorStatusGetter& dcMotor = dcMock.get();
		IMotorStatusGetter& servo = servoMock.get();
		IGPS& gps = gpsMock.get();

		//Set function return values
		const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
		const std::vector<Coordinate> path_vector = {
			Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
		};
		//Path to save file
		boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
			"JSON\\Unit_tests\\");
		fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
		fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
		fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
		fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
		fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		const std::string timestamp = std::to_string(ms.count());

		//Unit under test
		JSONTransmitter uut(dcMotor, servo, gps, filepath.string());

		//Act - Save the file to the specified folder
		uut.NavAcquisition(nav_ptr);
		uut.TransmitFromNav(timestamp);

		//Read from file to build string for Assert step
		std::ifstream fromNav(filepath.string() + "fromNav.json");
		nlohmann::json from_nav_json;
		fromNav >> from_nav_json;

		//Create expected string
		const std::string expected = timestamp;
		const std::string from_nav_result = from_nav_json.at("Timestamp_").dump();

		//Assert
		BOOST_REQUIRE(from_nav_result == expected);
	}
	
BOOST_AUTO_TEST_SUITE_END()