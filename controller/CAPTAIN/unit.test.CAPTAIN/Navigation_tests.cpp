#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include "boost/fakeit.hpp"
#include <boost/filesystem/operations.hpp>
#include "CAPTAIN/ITransmitter.h"
#include "CAPTAIN/IGPS.h"
#include "CAPTAIN/IAutopilot.h"
#include "CAPTAIN/Navigation.h"
#include "CAPTAIN/Pose.h"
#include "CAPTAIN/Coordinate.h"

BOOST_AUTO_TEST_SUITE(Navigation_tests)

//This test suite encapsulates all cases of the PeformTask function overload with Start and Stop
BOOST_AUTO_TEST_SUITE(Navigation_PerformTask_start_stop_tests)

//This test will verify that autopilot_.Run() will be called with 0.218 as the xte when:
//The ship is ahead of the previous and next points in relation to the heading between those points
//The ship is on the right side of the line connecting the previous and next point
//Result for comparison taken from http://geo.javawa.nl/coordcalc/index_en.html
BOOST_AUTO_TEST_CASE(Navigation_Start_xte_case_1)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.011, 10.015), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "0.218";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 4; i++)
		{
			if (j == 3 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}
		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);
	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 0.544 as the xte when:
//The ship is between the previous and next points in relation to the heading between those points
//The ship is on the right side of the line connecting the previous and next point
//Result for comparison taken from http://geo.javawa.nl/coordcalc/index_en.html
BOOST_AUTO_TEST_CASE(Navigation_Start_xte_case_2)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.0, 10.01), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "0.544";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 4; i++)
		{
			if (j == 3 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);
	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 0.120 as the xte when:
//The ship is behind the previous and next points in relation to the heading between those points
//The ship is on the right side of the line connecting the previous and next point
//Result for comparison taken from http://geo.javawa.nl/coordcalc/index_en.html
BOOST_AUTO_TEST_CASE(Navigation_Start_xte_case_3)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(55.9989, 10.0011), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "0.120";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 4; i++)
		{
			if (j == 3 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);
	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 0.087 as the xte when:
//The ship is behind the previous and next points in relation to the heading between those points
//The ship is on the left side of the line connecting the previous and next point
//Result for comparison taken from http://geo.javawa.nl/coordcalc/index_en.html
BOOST_AUTO_TEST_CASE(Navigation_Start_xte_case_4)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(55.9994, 9.9978), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "0.087";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 4; i++)
		{
			if (j == 3 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);
	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 0.544 as the xte when:
//The ship is between the previous and next points in relation to the heading between those points
//The ship is on the left side of the line connecting the previous and next point
//Result for comparison taken from http://geo.javawa.nl/coordcalc/index_en.html
BOOST_AUTO_TEST_CASE(Navigation_Start_xte_case_5)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.01, 10.0), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "0.544";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 4; i++)
		{
			if (j == 3 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 0.163 as the xte when:
//The ship is ahead of the previous and next points in relation to the heading between those points
//The ship is on the left side of the line connecting the previous and next point
//Result for comparison taken from http://geo.javawa.nl/coordcalc/index_en.html
BOOST_AUTO_TEST_CASE(Navigation_Start_xte_case_6)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.012, 10.009), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "0.163";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 4; i++)
		{
			if (j == 3 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "L" as the direction when:
//The next point is in the first quadrant
//The ship's absolute heading lies in the first quadrant
//The ship's absolute heading is greater than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_1_case_1)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 30);
	const std::string time = "90";
	boost::filesystem::path filepath = boost::filesystem::current_path();
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "L";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}


//This test will verify that autopilot_.Run() will be called with "L" as the direction when:
//The next point is in the first quadrant
//The ship's absolute heading lies in the fourth quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_1_case_2)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 135);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 135);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "L";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "L" as the direction when:
//The next point is in the first quadrant
//The ship's absolute heading lies in the third quadrant
//The ship's absolute heading is smaller than pi plus the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_1_case_3)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 200);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 200);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "L";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "R" as the direction when:
//The next point is in the first quadrant
//The ship's absolute heading lies in the third quadrant
//The ship's absolute heading is greater than pi plus the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_1_case_4)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 210);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 210);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "R";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "R" as the direction when:
//The next point is in the first quadrant
//The ship's absolute heading lies in the second quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_1_case_5)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 315);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 315);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "R";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "R" as the direction when:
//The next point is in the first quadrant
//The ship's absolute heading lies in the first quadrant
//The ship's absolute heading is smaller than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_1_case_6)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 20);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 20);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "R";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "L" as the direction when:
//The next point is in the second quadrant
//The ship's absolute heading lies in the second quadrant
//The ship's absolute heading is greater than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_2_case_1)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 340);
	const Pose pose_new = Pose(Coordinate(56.00001, 10.0), 340);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "L";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "L" as the direction when:
//The next point is in the second quadrant
//The ship's absolute heading lies in the first quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_2_case_2)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 45);
	const Pose pose_new = Pose(Coordinate(56.00001, 10.0), 45);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "L";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "L" as the direction when:
//The next point is in the second quadrant
//The ship's absolute heading lies in the fourth quadrant
//The ship's absolute heading is smaller than the absolute heading from the ship to the next point minus pi
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_2_case_3)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 140);
	const Pose pose_new = Pose(Coordinate(56.00001, 10.0), 140);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "L";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "R" as the direction when:
//The next point is in the second quadrant
//The ship's absolute heading lies in the fourth quadrant
//The ship's absolute heading is greater than the absolute heading from the ship to the next point minus pi
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_2_case_4)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 160);
	const Pose pose_new = Pose(Coordinate(56.00001, 10.0), 160);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "R";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "R" as the direction when:
//The next point is in the second quadrant
//The ship's absolute heading lies in the third quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_2_case_5)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 225);
	const Pose pose_new = Pose(Coordinate(56.00001, 10.0), 225);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "R";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "R" as the direction when:
//The next point is in the second quadrant
//The ship's absolute heading lies in the second quadrant
//The ship's absolute heading is smaller than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_2_case_6)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 320);
	const Pose pose_new = Pose(Coordinate(56.00001, 10.0), 320);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "R";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "L" as the direction when:
//The next point is in the third quadrant
//The ship's absolute heading lies in the third quadrant
//The ship's absolute heading is greater than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_3_case_1)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 210);
	const Pose pose_new = Pose(Coordinate(56.0, 9.99999), 210);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "L";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "L" as the direction when:
//The next point is in the third quadrant
//The ship's absolute heading lies in the second quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_3_case_2)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 315);
	const Pose pose_new = Pose(Coordinate(56.0, 9.99999), 315);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "L";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "L" as the direction when:
//The next point is in the third quadrant
//The ship's absolute heading lies in the first quadrant
//The ship's absolute heading is smaller than the absolute heading from the ship to the next point minus pi
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_3_case_3)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 20);
	const Pose pose_new = Pose(Coordinate(56.0, 9.99999), 20);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "L";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "R" as the direction when:
//The next point is in the third quadrant
//The ship's absolute heading lies in the first quadrant
//The ship's absolute heading is greater than the absolute heading from the ship to the next point minus pi
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_3_case_4)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.0, 9.99999), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "R";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "R" as the direction when:
//The next point is in the third quadrant
//The ship's absolute heading lies in the fourth quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_3_case_5)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 135);
	const Pose pose_new = Pose(Coordinate(56.0, 9.99999), 135);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "R";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "R" as the direction when:
//The next point is in the third quadrant
//The ship's absolute heading lies in the third quadrant
//The ship's absolute heading is smaller than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_3_case_6)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 200);
	const Pose pose_new = Pose(Coordinate(56.0, 9.99999), 200);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "R";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "L" as the direction when:
//The next point is in the fourth quadrant
//The ship's absolute heading lies in the fourth quadrant
//The ship's absolute heading is greater than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_4_case_1)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 150);
	const Pose pose_new = Pose(Coordinate(55.99999, 10.0), 150);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "L";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "L" as the direction when:
//The next point is in the fourth quadrant
//The ship's absolute heading lies in the third quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_4_case_2)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 225);
	const Pose pose_new = Pose(Coordinate(55.99999, 10.0), 225);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "L";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "L" as the direction when:
//The next point is in the fourth quadrant
//The ship's absolute heading lies in the second quadrant
//The ship's absolute heading is smaller than pi plus the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_4_case_3)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 320);
	const Pose pose_new = Pose(Coordinate(55.99999, 10.0), 320);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "L";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "R" as the direction when:
//The next point is in the fourth quadrant
//The ship's absolute heading lies in the second quadrant
//The ship's absolute heading is greater than pi plus the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_4_case_4)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 330);
	const Pose pose_new = Pose(Coordinate(55.99999, 10.0), 330);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "R";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "R" as the direction when:
//The next point is in the fourth quadrant
//The ship's absolute heading lies in the first quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_4_case_5)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 45);
	const Pose pose_new = Pose(Coordinate(55.99999, 10.0), 45);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "R";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with "R" as the direction when:
//The next point is in the fourth quadrant
//The ship's absolute heading lies in the fourth quadrant
//The ship's absolute heading is smaller than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_direction_to_steer_quadrant_4_case_6)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 140);
	const Pose pose_new = Pose(Coordinate(55.99999, 10.0), 140);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "R";
		std::string run_result = " ";

		int j = 0;

		for (int i = 0; run_result.compare(" ") == 0; i++)
		{
			if (j == 4)
			{
				run_result = telegram[i];
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			run_result._Equal(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 29.3 as orig_to_dest_bearing when:
//The next point is in the first quadrant
//Result for comparison taken from https://www.movable-type.co.uk/scripts/latlong.html
BOOST_AUTO_TEST_CASE(Navigation_Start_Bearing_origin_to_destination_case_1)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "29.3";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 9; i++)
		{
			if (j == 8 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 150.7 as orig_to_dest_bearing when:
//The next point is in the fourth quadrant
//Result for comparison taken from https://www.movable-type.co.uk/scripts/latlong.html
BOOST_AUTO_TEST_CASE(Navigation_Start_Bearing_origin_to_destination_case_2)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "150.7";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 9; i++)
		{
			if (j == 8 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 209.3 as orig_to_dest_bearing when:
//The next point is in the third quadrant
//Result for comparison taken from https://www.movable-type.co.uk/scripts/latlong.html
BOOST_AUTO_TEST_CASE(Navigation_Start_Bearing_origin_to_destination_case_3)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "209.3";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 9; i++)
		{
			if (j == 8 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 330.7 as orig_to_dest_bearing when:
//The next point is in the second quadrant
//Result for comparison taken from https://www.movable-type.co.uk/scripts/latlong.html
BOOST_AUTO_TEST_CASE(Navigation_Start_Bearing_origin_to_destination_case_4)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "330.7";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 9; i++)
		{
			if (j == 8 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 7 as dest when:
//The next point is waypoint #7
BOOST_AUTO_TEST_CASE(Navigation_Start_destination)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_2 = Pose(Coordinate(56.000078, 10.000078), 30);
	const Pose pose_3 = Pose(Coordinate(56.000156, 10.000156), 30);
	const Pose pose_4 = Pose(Coordinate(56.000234, 10.000234), 30);
	const Pose pose_5 = Pose(Coordinate(56.000312, 10.000312), 30);
	const Pose pose_6 = Pose(Coordinate(56.000390, 10.000390), 30);

	//Since we're traversing a path, we need to return getpose many times. Once to calculate a path, and
	//then two for each subsequent Start call. We're shooting for a "next waypoint" of 7, so we need
	//6 calls to Start. 6*2 + 1 (from calculating the path) is 13 total calls.
	fakeit::When(Method(gpsMock, GetPose)).Return
	(pose_start, pose_start, pose_start, pose_2, pose_2, pose_3, pose_3, pose_4, pose_4, pose_5, pose_5, pose_6, pose_6);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).AlwaysReturn();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start several times. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);

	//Assert
	//Check if Run() was ever called with a "next waypoint" number of 5.
	//Run will be called many times in this test, but with incrementing "next waypoint" numbers, since
	//the current position is faked incrementally.
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "7";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 11; i++)
		{
			if (j == 10 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			stod(expected) == stod(run_result);

	}));
}

//This test will verify that autopilot_.Run() will be called with 4 as the curr_to_dest_relative_bearing when:
//The next point is in the first quadrant
//The ship's absolute heading lies in the first quadrant
//The ship's absolute heading is greater than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_1_case_1)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 30);
	const std::string time = "90";
	boost::filesystem::path filepath = boost::filesystem::current_path();
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "4";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}


//This test will verify that autopilot_.Run() will be called with 109 as the curr_to_dest_relative_bearing when:
//The next point is in the first quadrant
//The ship's absolute heading lies in the fourth quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_1_case_2)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 135);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 135);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "109";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 174 as the curr_to_dest_relative_bearing when:
//The next point is in the first quadrant
//The ship's absolute heading lies in the third quadrant
//The ship's absolute heading is smaller than pi plus the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_1_case_3)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 200);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 200);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "174";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 176 as the curr_to_dest_relative_bearing when:
//The next point is in the first quadrant
//The ship's absolute heading lies in the third quadrant
//The ship's absolute heading is greater than pi plus the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_1_case_4)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 210);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 210);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "176";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 71 as the curr_to_dest_relative_bearing when:
//The next point is in the first quadrant
//The ship's absolute heading lies in the second quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_1_case_5)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 315);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 315);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "71";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 6 as the curr_to_dest_relative_bearing when:
//The next point is in the first quadrant
//The ship's absolute heading lies in the first quadrant
//The ship's absolute heading is smaller than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_1_case_6)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 20);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 20);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "6";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 13 as the curr_to_dest_relative_bearing when:
//The next point is in the second quadrant
//The ship's absolute heading lies in the second quadrant
//The ship's absolute heading is greater than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_2_case_1)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 340);
	const Pose pose_new = Pose(Coordinate(56.00001, 10.0), 340);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "13";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 78 as the curr_to_dest_relative_bearing when:
//The next point is in the second quadrant
//The ship's absolute heading lies in the first quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_2_case_2)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 45);
	const Pose pose_new = Pose(Coordinate(56.00001, 10.0), 45);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "78";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 173 as the curr_to_dest_relative_bearing when:
//The next point is in the second quadrant
//The ship's absolute heading lies in the fourth quadrant
//The ship's absolute heading is smaller than the absolute heading from the ship to the next point minus pi
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_2_case_3)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 140);
	const Pose pose_new = Pose(Coordinate(56.00001, 10.0), 140);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "173";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 167 as the curr_to_dest_relative_bearing when:
//The next point is in the second quadrant
//The ship's absolute heading lies in the fourth quadrant
//The ship's absolute heading is greater than the absolute heading from the ship to the next point minus pi
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_2_case_4)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 160);
	const Pose pose_new = Pose(Coordinate(56.00001, 10.0), 160);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "167";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 102 as the curr_to_dest_relative_bearing when:
//The next point is in the second quadrant
//The ship's absolute heading lies in the third quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_2_case_5)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 225);
	const Pose pose_new = Pose(Coordinate(56.00001, 10.0), 225);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "102";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 7 as the curr_to_dest_relative_bearing when:
//The next point is in the second quadrant
//The ship's absolute heading lies in the second quadrant
//The ship's absolute heading is smaller than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_2_case_6)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 320);
	const Pose pose_new = Pose(Coordinate(56.00001, 10.0), 320);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "7";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 4 as the curr_to_dest_relative_bearing when:
//The next point is in the third quadrant
//The ship's absolute heading lies in the third quadrant
//The ship's absolute heading is greater than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_3_case_1)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 210);
	const Pose pose_new = Pose(Coordinate(56.0, 9.99999), 210);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "4";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 109 as the curr_to_dest_relative_bearing when:
//The next point is in the third quadrant
//The ship's absolute heading lies in the second quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_3_case_2)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 315);
	const Pose pose_new = Pose(Coordinate(56.0, 9.99999), 315);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "109";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 174 as the curr_to_dest_relative_bearing when:
//The next point is in the third quadrant
//The ship's absolute heading lies in the first quadrant
//The ship's absolute heading is smaller than the absolute heading from the ship to the next point minus pi
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_3_case_3)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 20);
	const Pose pose_new = Pose(Coordinate(56.0, 9.99999), 20);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "174";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 176 as the curr_to_dest_relative_bearing when:
//The next point is in the third quadrant
//The ship's absolute heading lies in the first quadrant
//The ship's absolute heading is greater than the absolute heading from the ship to the next point minus pi
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_3_case_4)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.0, 9.99999), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "176";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 71 as the curr_to_dest_relative_bearing when:
//The next point is in the third quadrant
//The ship's absolute heading lies in the fourth quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_3_case_5)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 135);
	const Pose pose_new = Pose(Coordinate(56.0, 9.99999), 135);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "71";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 6 as the curr_to_dest_relative_bearing when:
//The next point is in the third quadrant
//The ship's absolute heading lies in the third quadrant
//The ship's absolute heading is smaller than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_3_case_6)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 200);
	const Pose pose_new = Pose(Coordinate(56.0, 9.99999), 200);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "6";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 2.7 as the curr_to_dest_relative_bearing when:
//The next point is in the fourth quadrant
//The ship's absolute heading lies in the fourth quadrant
//The ship's absolute heading is greater than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_4_case_1)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 150);
	const Pose pose_new = Pose(Coordinate(55.99999, 10.0), 150);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "2.7";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 78 as the curr_to_dest_relative_bearing when:
//The next point is in the fourth quadrant
//The ship's absolute heading lies in the third quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_4_case_2)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 225);
	const Pose pose_new = Pose(Coordinate(55.99999, 10.0), 225);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "78";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 173 as the curr_to_dest_relative_bearing when:
//The next point is in the fourth quadrant
//The ship's absolute heading lies in the second quadrant
//The ship's absolute heading is smaller than pi plus the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_4_case_3)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 320);
	const Pose pose_new = Pose(Coordinate(55.99999, 10.0), 320);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "173";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 177 as the curr_to_dest_relative_bearing when:
//The next point is in the fourth quadrant
//The ship's absolute heading lies in the second quadrant
//The ship's absolute heading is greater than pi plus the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_4_case_4)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 330);
	const Pose pose_new = Pose(Coordinate(55.99999, 10.0), 330);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "177";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 102 as the curr_to_dest_relative_bearing when:
//The next point is in the fourth quadrant
//The ship's absolute heading lies in the first quadrant
//The absolute difference between the ship's absolute heading and the the absolute heading
//from the ship to the next point is less than pi
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_4_case_5)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 45);
	const Pose pose_new = Pose(Coordinate(55.99999, 10.0), 45);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "102";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 7 as the curr_to_dest_relative_bearing when:
//The next point is in the fourth quadrant
//The ship's absolute heading lies in the fourth quadrant
//The ship's absolute heading is smaller than the absolute heading from the ship to the next point
BOOST_AUTO_TEST_CASE(Navigation_Start_steer_heading_quadrant_4_case_6)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 140);
	const Pose pose_new = Pose(Coordinate(55.99999, 10.0), 140);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "7";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 12; i++)
		{
			if (j == 11 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 26.0 as orig_to_dest_bearing when:
//The next point is in the first quadrant
//Result for comparison taken from https://www.movable-type.co.uk/scripts/latlong.html
BOOST_AUTO_TEST_CASE(Navigation_Start_Heading_current_to_destination_case_1)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "26.0";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 14; i++)
		{
			if (j == 13 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 154.0 as orig_to_dest_bearing when:
//The next point is in the fourth quadrant
//Result for comparison taken from https://www.movable-type.co.uk/scripts/latlong.html
BOOST_AUTO_TEST_CASE(Navigation_Start_Relative_Bearing_current_to_destination_case_2)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "154.0";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 14; i++)
		{
			if (j == 13 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 212.3 as orig_to_dest_bearing when:
//The next point is in the third quadrant
//Result for comparison taken from https://www.movable-type.co.uk/scripts/latlong.html
BOOST_AUTO_TEST_CASE(Navigation_Start_Relative_Bearing_current_to_destination_case_3)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 55.99, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "212.3";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 14; i++)
		{
			if (j == 13 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

//This test will verify that autopilot_.Run() will be called with 327.7 as orig_to_dest_bearing when:
//The next point is in the second quadrant
//Result for comparison taken from https://www.movable-type.co.uk/scripts/latlong.html
BOOST_AUTO_TEST_CASE(Navigation_Start_Relative_Bearing_current_to_destination_case_4)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 9.99 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_new = Pose(Coordinate(56.0, 10.00001), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose_start, pose_new, pose_new);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);

	//Assert
	//Check what Run() was called with, and whether the telegram direction matches the expected direction
	fakeit::Verify(Method(autopilotMock, Run).Matching([](std::string telegram)
	{
		std::string expected = "327.7";
		std::string run_result;

		int j = 0;

		for (int i = 0; j < 14; i++)
		{
			if (j == 13 && telegram[i] != ',')
			{
				run_result.append(1, telegram[i]);
			}
			if (telegram[i] == ',')
			{
				j++;
			}
		}

		return
			0.95*stod(expected) < stod(run_result) && stod(run_result) < 1.05*stod(expected);

	})).Exactly(1);
}

BOOST_AUTO_TEST_CASE(Navigation_Stop)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	fakeit::When(Method(gpsMock, GetPose)).Return(Pose(Coordinate(56.0, 10.0), 7.45));
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Stop)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act - Perform the stop task
	uut.PerformTask(Stop);

	//Assert
	fakeit::Verify(Method(autopilotMock, Stop)).Exactly(1);
}

BOOST_AUTO_TEST_SUITE_END()

//This test suite encapsulates all cases of the PeformTask function overload with CalcP2P
//It seems inappropriate to test the contents of the path_ vector here, since this vector is private, and is tested on
//In the Transmitter, where it is used. Navigation only uses path_ to update completed_path_, and this is tested in
//the dist test of the PerformTask_start_continue_stop suite. Further testing of the output can well be part of
//subsequent integration and end-to-end tests.
BOOST_AUTO_TEST_SUITE(Navigation_PerformTask_calculate_p2p_path_tests)

//This test will verify that PerformTask with CalcP2P and a TargetPosition as its arguments will not call Autopilot.Run(),
//Autopilot.Stop(), but will call Transmitter.TransmitFromNav() exactly once.
BOOST_AUTO_TEST_CASE(Navigation_Calculate_Point_to_Point)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose = Pose(Coordinate(56.0, 10.0), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose);
	fakeit::When(Method(autopilotMock, Stop)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act - Perform the stop task
	uut.PerformTask(CalcP2P, target);

	//Assert
	fakeit::Verify(Method(autopilotMock, Stop)).Exactly(0);
	fakeit::Verify(Method(autopilotMock, Run)).Exactly(0);
	fakeit::Verify(Method(transmitterMock, TransmitFromNav)).Exactly(1);
}

BOOST_AUTO_TEST_SUITE_END()

//This test suite encapsulates all cases of the PeformTask function overload with CalcCoverageRectangle
//It seems inappropriate to test the contents of the path_ vector here, since this vector is private, and is tested on
//In the Transmitter, where it is used. Navigation only uses path_ to update completed_path_, and this is tested in
//the dist test of the PerformTask_start_continue_stop suite. Further testing of the output can well be part of
//subsequent integration and end-to-end tests.
BOOST_AUTO_TEST_SUITE(Navigation_PerformTask_calculate_coverage_rectangle_path_tests)

//This test will verify that PerformTask with CalcP2P and a TargetPosition as its arguments will not call Autopilot.Run(),
//Autopilot.Stop(), but will call Transmitter.TransmitFromNav() exactly once.
BOOST_AUTO_TEST_CASE(Navigation_Calculate_Coverage_Rectangle)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const CoverageRectangle coverage_rectangle = CoverageRectangle{ { 56.01, 10.01 }, { 56.02, 10.02 } };
	const Pose pose = Pose(Coordinate(56.0, 10.0), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose);
	fakeit::When(Method(autopilotMock, Stop)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act - Perform the stop task
	uut.PerformTask(CalcCoverageRectangle, coverage_rectangle);

	//Assert
	fakeit::Verify(Method(autopilotMock, Stop)).Exactly(0);
	fakeit::Verify(Method(autopilotMock, Run)).Exactly(0);
	fakeit::Verify(Method(transmitterMock, TransmitFromNav)).Exactly(1);
}

BOOST_AUTO_TEST_SUITE_END()

//This test suite encapsulates all cases of the GetNavData function. Since it relies on real-time calls to a timer,
//the Ete_ cannot be tested here.
BOOST_AUTO_TEST_SUITE(Navigation_GetNavData_tests)

//This test will verify that a call to GetNavData after a CoverageRectangle has been calculcated will output a
//path_ vector with a front element equaling the pose input
BOOST_AUTO_TEST_CASE(Navigation_Get_Navigation_Data_path_)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const CoverageRectangle coverage_rectangle = CoverageRectangle{ { 56.01, 10.01 },{ 56.02, 10.02 } };
	const Pose pose = Pose(Coordinate(56.0, 10.0), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose);
	fakeit::When(Method(autopilotMock, Stop)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act - Perform the stop task
	uut.PerformTask(CalcCoverageRectangle, coverage_rectangle);
	NavigationData nav_data = uut.GetNavData();

	//Assert
	BOOST_REQUIRE(
		nav_data.Path_.front().Latitude_ == 56 && nav_data.Path_.front().Longitude_ == 10
	);
}

//This test will verify that a call to GetNavData after a CoverageRectangle has been calculcated will output a
//completed_path_ vector which is empty since start has not yet been called
BOOST_AUTO_TEST_CASE(Navigation_Get_Navigation_Data_completed_path_)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const CoverageRectangle coverage_rectangle = CoverageRectangle{ { 56.01, 10.01 },{ 56.02, 10.02 } };
	const Pose pose = Pose(Coordinate(56.0, 10.0), 30);
	fakeit::When(Method(gpsMock, GetPose)).Return(pose);
	fakeit::When(Method(autopilotMock, Stop)).Return();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act - Perform the stop task
	uut.PerformTask(CalcCoverageRectangle, coverage_rectangle);
	NavigationData nav_data = uut.GetNavData();

	//Assert
	BOOST_REQUIRE(
		nav_data.Completed_path_.size() == 0
	);
}

//This test will verify that a call to GetNavData after a P2P Path has been calculcated and a number of points have been
//traversed will output a Progress_ of 10%
BOOST_AUTO_TEST_CASE(Navigation_Get_Navigation_Data_Progress_)
{
	//Arrange
	//Link mocks to interfaces
	fakeit::Mock<IGPS> gpsMock;
	fakeit::Mock<ITransmitter> transmitterMock;
	fakeit::Mock<IAutopilot> autopilotMock;

	//Instantiate mocks
	IGPS& gps = gpsMock.get();
	ITransmitter& transmitter = transmitterMock.get();
	IAutopilot& auto_pilot = autopilotMock.get();

	//Set function return values
	const TargetPosition target = TargetPosition({ 56.01, 10.01 });
	const Pose pose_start = Pose(Coordinate(56.0, 10.0), 30);
	const Pose pose_2 = Pose(Coordinate(56.000078, 10.000078), 30);
	const Pose pose_3 = Pose(Coordinate(56.000156, 10.000156), 30);
	const Pose pose_4 = Pose(Coordinate(56.000234, 10.000234), 30);
	const Pose pose_5 = Pose(Coordinate(56.000312, 10.000312), 30);
	const Pose pose_6 = Pose(Coordinate(56.000390, 10.000390), 30);
	const Pose pose_7 = Pose(Coordinate(56.000468, 10.000468), 30);
	const Pose pose_8 = Pose(Coordinate(56.000547, 10.000547), 30);
	const Pose pose_9 = Pose(Coordinate(56.000625, 10.000625), 30);
	const Pose pose_10 = Pose(Coordinate(56.000703, 10.000703), 30);
	const Pose pose_11 = Pose(Coordinate(56.000781, 10.000781), 30);
	const Pose pose_12 = Pose(Coordinate(56.000859, 10.000859), 30);
	const Pose pose_13 = Pose(Coordinate(56.000938, 10.000937), 30);
	//NB: Notice how the very last pose has a difference in change between the coordinates; 938 and 937. This is due to
	//the fact that we're using rhumb lines, and thus moving ever so slightly towards the pole. All rhumb lines (which
	//are not along a constant longitude) all eventually end at the north or south pole, and this tiny variation in
	//a test is a clue to this fact.

	//Since we're traversing a path, we need to return getpose many times. Once to calculate a path, and
	//then two for each subsequent Start call.
	fakeit::When(Method(gpsMock, GetPose)).Return
	(pose_start, pose_start, pose_start, pose_2, pose_2, pose_3, pose_3, pose_4, pose_4, pose_5, pose_5, pose_6, pose_6,
		pose_7, pose_7, pose_8, pose_8, pose_9, pose_9, pose_10, pose_10, pose_11, pose_11, pose_12, pose_12, pose_13, pose_13);
	fakeit::When(Method(gpsMock, GetSpeed)).AlwaysReturn(0);
	fakeit::When(Method(autopilotMock, Run)).AlwaysReturn();
	fakeit::Fake(Method(transmitterMock, NavAcquisition));
	fakeit::Fake(Method(transmitterMock, TransmitFromNav));

	//Unit under test
	Navigation uut = Navigation(gps, transmitter, auto_pilot);

	//Act
	//First calculate a path, then start several times. The result is that autopilot_.Run() will be called
	uut.PerformTask(CalcP2P, target);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);
	uut.PerformTask(Start);

	//Finally make the call to GetNavData
	NavigationData nav_data = uut.GetNavData();

	//Assert
	//Check if nav_data.progress
	double expected = 0.10;

	//Assert
	BOOST_REQUIRE(0.95*expected < nav_data.Progress_ && nav_data.Progress_ < 1.05*expected);
}

BOOST_AUTO_TEST_SUITE_END()

//This test suite encapsulates all cases of the SetParameters function
//Set functions cannot be unit tested, but this function can be verified in an integration test with Transmitter
BOOST_AUTO_TEST_SUITE(Navigation_Set_Parameters_tests)

BOOST_AUTO_TEST_CASE(Navigation_Set_Parameters_unable_to_test)
{
	BOOST_REQUIRE(1 == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
