#include <thread>
#include <chrono>
#include "CAPTAIN/SimpleSerial.h"
#include "CAPTAIN/PiGpio.h"
#include "CAPTAIN/Ublox_neo7m.h"
#include "CAPTAIN/Servo.h"
#include "CAPTAIN/DCMotor.h"
#include "CAPTAIN/Autopilot.h"
#include "CAPTAIN/JSONTransmitter.h"
#include "CAPTAIN/Navigation.h"
#include "CAPTAIN/JSONReceiver.h"

int main()
{
	const std::string port = "/dev/ttyS0";
	const unsigned int baud_rate = 9600;
	SimpleSerial serial(port, baud_rate);
	Ublox_neo7m gps = Ublox_neo7m(serial);
	std::thread gps_thread = gps.Setup();

	gps_thread.detach();

	PiGpio servoGPIO = PiGpio();
	Servo rudder = Servo(servoGPIO);
	PiGpio motorGPIO = PiGpio();
	DCMotor thruster = DCMotor(motorGPIO);
	Autopilot auto_pilot = Autopilot(rudder, thruster);

	boost::filesystem::path filepath = boost::filesystem::current_path().parent_path().parent_path().parent_path().append(
		"website\\savedData\\");
	JSONTransmitter transmitter = JSONTransmitter(rudder, thruster, gps, filepath.string());
	Navigation nav = Navigation(gps, transmitter, auto_pilot);
	JSONReceiver receiver = JSONReceiver(nav, auto_pilot, filepath.string());

	while (true)
	{
		receiver.ReceiveActiveParam();
		receiver.ReceiveToNav();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	gps.StopThread();
}
