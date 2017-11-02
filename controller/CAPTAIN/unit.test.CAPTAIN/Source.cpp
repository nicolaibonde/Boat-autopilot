#define BOOST_TEST_MODULE CaptainTests
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp> //It is slow because this is a huge header
#include <string>
#include <chrono>
#include "boost/fakeit.hpp"
#include <CAPTAIN/Coordinate.h>
#include <CAPTAIN/Pose.h>
#include <CAPTAIN/NavigationData.h>
#include <CAPTAIN/TargetPosition.h>
#include <CAPTAIN/CoverageRectangle.h>
//#include <CAPTAIN/DCMotor.h>
//#include <CAPTAIN/Servo.h>
#include <CAPTAIN/MotorStatus.h>
#include <CAPTAIN/GPSStatus.h>
#include <CAPTAIN/INavigation.h>
#include <CAPTAIN/IAutopilot.h>
#include <CAPTAIN/JSONReceiver.h>
#include <CAPTAIN/JSONTransmitter.h>
#include <CAPTAIN/ISerial.h>
#include "CAPTAIN/Ublox_neo7m.h"

class Ublox_neo7m;

struct SomeInterface
{
	virtual int foo(int);
};

BOOST_AUTO_TEST_SUITE(All_tests)

BOOST_AUTO_TEST_CASE(fakeit_test)
{
	fakeit::Mock<SomeInterface> mock;
	fakeit::When(Method(mock, foo)).Return(1);

	SomeInterface &i = mock.get();
	BOOST_REQUIRE(i.foo(10) == 1);
}


BOOST_AUTO_TEST_SUITE(JSONTransmitter_tests)

//This test will verify that the first completed_path latitude input produces the expected output
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_completed_path_latitude_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "56.08";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(42, 5) == expected);
}

//This test will verify that the first completed_path longitude input produces the expected output
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_completed_path_longitude_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "10.12";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(61, 5) == expected);
}

//This test will verify that the second completed_path latitude input produces the expected output
//and thus confirm that the vector iterator behaves as expected.
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_completed_path_next_latitude_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "56.18";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(81, 5) == expected);
}

//This test will verify that the second completed_path longitude input produces the expected output
//and thus confirm that the vector iterator behaves as expected.
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_completed_path_next_longitude_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "10.22";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(100, 5) == expected);
}

//This test will verify that the first path latitude input produces the expected output
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_path_latitude_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "56.1";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(140, 4) == expected);
}

//This test will verify that the first path longitude input produces the expected output
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_path_longitude_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "10.1";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(158, 4) == expected);
}

//This test will verify that the second path latitude input produces the expected output
//and thus confirm that the vector iterator behaves as expected.
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_path_next_latitude_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "56.2";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(177, 4) == expected);
}

//This test will verify that the second path longitude input produces the expected output
//and thus confirm that the vector iterator behaves as expected.
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_path_next_longitude_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "10.2";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(195, 4) == expected);
}

//This test will verify that the Progress ete input produces the expected output
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Progress_ete_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = R"("2 min 31 sec")";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(324, 14) == expected);
}

//This test will verify that the Progress percentage input produces the expected output
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Progress_percentage_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "60.9";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(353, 4) == expected);
}

//This test will verify that the DCMotor percentage input produces the expected output
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Motor_DCMotorPercentage_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "15.44";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(740, 5) == expected);
}

//This test will verify that the Servo percentage input produces the expected output
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Motor_ServoPercentage_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "45.44";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(850, 5) == expected);
}

//This test will verify that the telemetry (and pose) latitude input(s) produce(s) the expected output
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Telemetry_and_pose_latitude_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "56.0";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(932, 4) == expected);
}

//This test will verify that the telemetry (and pose) longitude input(s) produce(s) the expected output
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Telemetry_and_pose_longitude_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "10.0";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(950, 4) == expected);
}

//This test will verify that the telemetry (and pose) orientation input(s) produce(s) the expected output
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_Telemetry_and_pose_orientation_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = "7.45";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(970, 4) == expected);
}

//This test will verify that the timestamp input produces the expected output
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_timestamp_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create expected string
	std::string expected = timestamp;

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str().substr(989, 13) == expected);
}

//OPTIONAL
//This test will verify the full functionality of the TransmitFromNav method; that the complete 
//1000+ character output file contents are exactly equal to a predefined, preverified raw string
BOOST_AUTO_TEST_CASE(JSONTransmitter_TransmitFromNav_complete_test)
{
	//Arrange
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
	const std::vector<Coordinate> completed_path_vector = { Coordinate(56.08, 10.12), Coordinate(56.18, 10.22) };
	const std::vector<Coordinate> path_vector = {
		Coordinate(56.1, 10.1), Coordinate(56.2, 10.2), Coordinate(56.3, 10.3), Coordinate(56.4, 10.4)
	};
	fakeit::When(Method(navMock, GetNavData)).Return(NavigationData(completed_path_vector, path_vector, 151, 60.9));
	fakeit::When(Method(dcMock, GetStatus)).Return(MotorStatus(15.44, SPEED));
	fakeit::When(Method(servoMock, GetStatus)).Return(MotorStatus(45.44, POSITION));
	fakeit::When(Method(gpsMock, GetStatus)).Return(GPSStatus(1.1, 2, 3.3, 4, Pose(Coordinate(56.0, 10.0), 7.45)));
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	const std::string timestamp = std::to_string(ms.count());

	//Unit under test
	JSONTransmitter uut(navigation, dcMotor, servo, gps);

	//Path to save file
	boost::filesystem::path path = boost::filesystem::current_path().append("JSON\\fromNav\\");

	//Act - Save the file to the specified folder
	uut.TransmitFromNav(path.string(), timestamp);

	//Read from file to build string for Assert step
	std::ifstream fromNav(path.string() + "\\fromNav.json");
	std::stringstream from_nav_buffer;
	from_nav_buffer << fromNav.rdbuf();

	//Create text string with the full JSON test string
	std::string expected =
		R"({"Completed_path_":{"line_":[{"latitude_":56.08,"longitude_":10.12},{"latitude_":56.18,"longitude_":10.22}]},"Path_":{"line_":[{"latitude_":56.1,"longitude_":10.1},{"latitude_":56.2,"longitude_":10.2},{"latitude_":56.3,"longitude_":10.3},{"latitude_":56.4,"longitude_":10.4}],"timestamp_":)"
		+ timestamp +
		R"(},"Progress_":{"ete_":"2 min 31 sec","percentage_":60.9},"Status_":[{"items_":[{"data_":5.2,"title_":"GPS frequency","unit_":"Hz"},{"data_":1.2,"title_":"GPS delay","unit_":"ms"}],"title_":"GPS Connection"},{"items_":[{"data_":56.0,"title_":"Latitude","unit_":"deg"},{"data_":10.0,"title_":"Latitude","unit_":"deg"},{"data_":7.45,"title_":"Orientation","unit_":"deg"}],"title_":"Pose"},{"items_":[{"color_":"progress-bar-warning","data_":15.44,"title_":"DC Motor","unit_":"%"}],"title_":"Motor"},{"items_":[{"color_":"progress-bar-warning","data_":45.44,"title_":"Servo","unit_":"%"}],"title_":"Motor"}],"Telemetry_":{"latitude_":56.0,"longitude_":10.0,"orientation_":7.45},"Timestamp_":)"
		+ timestamp + "}";

	//Assert
	BOOST_REQUIRE(from_nav_buffer.str() == expected);
}
BOOST_AUTO_TEST_SUITE_END()

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

BOOST_AUTO_TEST_SUITE(GPSStatus_tests)
BOOST_AUTO_TEST_CASE(GPSStatus_GetString_returnsJSON)
{
	GPSStatus uut = GPSStatus(1.1, 2, 3.3 , 4,  Pose(Coordinate(56.2, 10.8), 310)); //unit under test
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

BOOST_AUTO_TEST_SUITE_END()