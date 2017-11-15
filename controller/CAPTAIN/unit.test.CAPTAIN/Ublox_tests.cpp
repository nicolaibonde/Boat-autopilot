#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "CAPTAIN/Ublox_neo7m.h"
#include "boost/fakeit.hpp"
#include "CAPTAIN/json.hpp"

//This is a manual stub that can run with threads.
class serialMock : public ISerial
{
public:

	serialMock()
	{
		read_return_ = "";
	}

	void SetReadReturn(std::string s)
	{
		read_return_ = s;
	}

	std::string ReadLine() override
	{
		return read_return_;
	}

	void WriteString(std::string) override
	{
	}

private:
	std::string read_return_;
};

void WaitForGPS(IGPS& uut)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	uut.StopThread();
}

BOOST_AUTO_TEST_SUITE(Ublox_tests)

	BOOST_AUTO_TEST_CASE(Ublox_lat_lon_test, *boost::unit_test::tolerance(0.001))
	{
		serialMock serial{};

		serial.SetReadReturn("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");

		Ublox_neo7m uut = Ublox_neo7m(serial);

		std::thread gps_thread = uut.Setup();


		gps_thread.detach();
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //wait for tread to hopefully have run atleast once
		uut.StopThread(); //Stop background thread gracefully
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //Wait for thread to hopefully have closed

		auto result = uut.GetPose();
		auto expected = Pose(Coordinate(48.1173, 11.5167), 0);

		BOOST_TEST(result.Coordinate_.Latitude_ == expected.Coordinate_.Latitude_);
		BOOST_TEST(result.Coordinate_.Longitude_ == expected.Coordinate_.Longitude_);
	}

	BOOST_AUTO_TEST_CASE(Ublox_test_wrong_checksum, *boost::unit_test::tolerance(0.001))
	{
		serialMock serial{};

		serial.SetReadReturn("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*43");

		Ublox_neo7m uut = Ublox_neo7m(serial);

		std::thread gps_thread = uut.Setup();


		gps_thread.detach();
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //wait for tread to hopefully have run atleast once
		uut.StopThread(); //Stop background thread gracefully
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //Wait for thread to hopefully have closed

		auto result_pose = uut.GetPose();
		double result_speed = uut.GetSpeed();
		auto expected_pose = Pose(Coordinate(0, 0), 0);
		double expected_speed = 0;

		BOOST_TEST(result_pose.Coordinate_.Latitude_ == expected_pose.Coordinate_.Latitude_);
		BOOST_TEST(result_pose.Coordinate_.Longitude_ == expected_pose.Coordinate_.Longitude_);
		BOOST_TEST(result_speed == expected_speed);
	}

	BOOST_AUTO_TEST_CASE(Ublox_test_unknown_sentence, *boost::unit_test::tolerance(0.000001))
	{
		serialMock serial{};

		serial.SetReadReturn("$GPZAS,*73");

		Ublox_neo7m uut = Ublox_neo7m(serial);

		std::thread gps_thread = uut.Setup();


		gps_thread.detach();
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //wait for tread to hopefully have run atleast once
		uut.StopThread(); //Stop background thread gracefully
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //Wait for thread to hopefully have closed

		auto result_pose = uut.GetPose();
		double result_speed = uut.GetSpeed();
		auto expected_pose = Pose(Coordinate(0, 0), 0);
		double expected_speed = 0;

		BOOST_TEST(result_pose.Coordinate_.Latitude_ == expected_pose.Coordinate_.Latitude_);
		BOOST_TEST(result_pose.Coordinate_.Longitude_ == expected_pose.Coordinate_.Longitude_);
		BOOST_TEST(result_speed == expected_speed);
	}

	BOOST_AUTO_TEST_CASE(Ublox_test_get_status, *boost::unit_test::tolerance(0.001))
	{
		serialMock serial{};

		serial.SetReadReturn("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");

		Ublox_neo7m uut = Ublox_neo7m(serial);

		std::thread gps_thread = uut.Setup();

		gps_thread.detach();
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); //wait for tread to hopefully have run atleast once
		uut.StopThread(); //Stop background thread gracefully
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //Wait for thread to hopefully have closed


		auto expected = nlohmann::json::parse(
			"{\"Status_\": [" + GPSStatus(1, 8, 0.9, 123519, Pose(Coordinate(48.1173, 11.5167), 0)).GetString() + "]}");
		auto result = nlohmann::json::parse("{\"Status_\": [" + uut.GetStatus().GetString() + "]}");

		//fix
		BOOST_TEST(expected["Status_"][0]["items_"][0]["data_"] == result["Status_"][0]["items_"][0]["data_"]);
		//Sat
		BOOST_TEST(expected["Status_"][0]["items_"][1]["data_"] == result["Status_"][0]["items_"][1]["data_"]);
		//hdop
		BOOST_TEST(expected["Status_"][0]["items_"][2]["data_"] == result["Status_"][0]["items_"][2]["data_"]);
		//timestamp	
		BOOST_TEST(expected["Status_"][0]["items_"][3]["data_"] == result["Status_"][0]["items_"][3]["data_"]);

		//Pose
		BOOST_TEST(expected["Status_"][1]["items_"][0]["data_"] == result["Status_"][1]["items_"][0]["data_"]);
		BOOST_TEST(static_cast<double>(expected["Status_"][1]["items_"][1]["data_"]) == static_cast<double>(result["Status_"][
			1]["items_"][1]["data_"]));
	}


	BOOST_AUTO_TEST_CASE(Ublox_speed_test, *boost::unit_test::tolerance(0.001))
	{
		serialMock serial{};

		serial.SetReadReturn("$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48");

		Ublox_neo7m uut = Ublox_neo7m(serial);

		std::thread gps_thread = uut.Setup();


		gps_thread.detach();
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //wait for tread to hopefully have run atleast once
		uut.StopThread(); //Stop background thread gracefully
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //Wait for thread to hopefully have closed

		auto result = uut.GetSpeed();
		auto expected = 10.2;


		BOOST_TEST(result == expected);
	}

	BOOST_AUTO_TEST_CASE(Ublox_empty_valid_gpgga_telegram, *boost::unit_test::tolerance(0.001))
	{
		serialMock serial{};

		serial.SetReadReturn("$GPGGA,,,,,,,,,,,,,*7A");

		Ublox_neo7m uut = Ublox_neo7m(serial);

		std::thread gps_thread = uut.Setup();


		gps_thread.detach();
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //wait for tread to hopefully have run atleast once
		uut.StopThread(); //Stop background thread gracefully
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //Wait for thread to hopefully have closed


		auto expected = nlohmann::json::parse(
			"{\"Status_\": [" + GPSStatus(0, 0, 0, 0, Pose(Coordinate(0, 0), 0)).GetString() + "]}");
		auto result = nlohmann::json::parse("{\"Status_\": [" + uut.GetStatus().GetString() + "]}");

		//fix
		BOOST_TEST(expected["Status_"][0]["items_"][0]["data_"] == result["Status_"][0]["items_"][0]["data_"]);
		//Sat
		BOOST_TEST(expected["Status_"][0]["items_"][1]["data_"] == result["Status_"][0]["items_"][1]["data_"]);
		//hdop
		BOOST_TEST(expected["Status_"][0]["items_"][2]["data_"] == result["Status_"][0]["items_"][2]["data_"]);
		//timestamp	
		BOOST_TEST(expected["Status_"][0]["items_"][3]["data_"] == result["Status_"][0]["items_"][3]["data_"]);

		//Pose
		BOOST_TEST(expected["Status_"][1]["items_"][0]["data_"] == result["Status_"][1]["items_"][0]["data_"]);
		BOOST_TEST(static_cast<double>(expected["Status_"][1]["items_"][1]["data_"]) == static_cast<double>(result["Status_"][
			1]["items_"][1]["data_"]));
	}

	BOOST_AUTO_TEST_CASE(Ublox_empty_valid_vtg_telegram)
	{
		serialMock serial{};

		serial.SetReadReturn("$GPVTG,,,,,,,,,N*30");

		Ublox_neo7m uut = Ublox_neo7m(serial);

		std::thread gps_thread = uut.Setup();

		gps_thread.detach();
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //wait for tread to hopefully have run atleast once
		uut.StopThread(); //Stop background thread gracefully
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //Wait for thread to hopefully have closed

		auto result = uut.GetSpeed();
		auto expected = 0;

		BOOST_TEST(result == expected);
	}


BOOST_AUTO_TEST_SUITE_END()
