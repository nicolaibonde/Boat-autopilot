#pragma once
#include <string>
#include "IMotorStatusGetter.h"
#include "ITransmitter.h"
#include "NavigationData.h"
#include "IGPS.h"

/**
* \brief   JSONTransmitter
* \details This is an implementation of the ITransmitter interface. \n
* This module is responsible for writing data to the fromNav.json file. \n
* Because Navigation and JSONTransmitter must call function in each other, JSONTransmitter.h has a forward declaration of INavigation,\n
* Navigation.h has a forward declaration of ITransmitter.h, JSONTransmitter.cpp includes INavigation.h, Navigation.cpp includes \n
* ITransmitter.h. The Navigation calls NavAcquisition in JSONTransmitter with a this-pointer parameter, and JSONTransmitter then \n
* saves this object pointer. Through this method, the two objects hold a reference and pointer to each other, respectively.\n
* \see Navigation.h
*/

class INavigation;

class JSONTransmitter :
	public ITransmitter
{
public:
	/**
	* \brief   Constructor
	* \param dc_motor This is a reference to the DCMotor
	* \param servo This is a reference to the servo
	* \param gps This is a reference to the gps
	* \param gps The filepath to save the fromNav.json file to
	*/
	JSONTransmitter(IMotorStatusGetter& dc_motor,
		IMotorStatusGetter& servo, IGPS& gps, const std::string filepath);

	/**
	* \brief   Destructor
	*/
	~JSONTransmitter();

	/**
	* \brief   Navigation acquisition called by Navigation constructor with an INavigation this-pointer to link Navigation and JSONTransmitter \n
	* correctly, since mutual references are not possible.
	* \param navigation A this-pointer to Navigation object
	*/
	void NavAcquisition(INavigation* navigation) override;

	/**
	* \brief   TransmitFromNavigation
	* \param timestamp The last timestamp set in the Navigation
	* \details Called by Navigation perform task in all three overloads with a timestamp string. \n
	* This functionality has the responsibility of collecting all relevant data from the necessary classes in the system, package \n
	* them in accordance with the fromNav.json protocol, and overwrite the json file with the new data. \n
	* The classes that information is pulled from are DCMotor, Servo, GPS, and Navigation. \n
	*/
	void TransmitFromNav(std::string const timestamp) override;

private:
	//! Pointer to navigation, to be able to get data from that class when TransmitFromNav() is called
	INavigation* navigation_;
	//! Reference to the instantiated DCMotor object
	IMotorStatusGetter& dc_motor_;
	//! Reference to the instantiated Servo object
	IMotorStatusGetter& servo_;
	//! String containing the system filepath to the JSON file folder that holds fromNav.JSON
	std::string filepath_;
	//! Reference to the instantiated GPS object
	IGPS& gps_;
	/**
	* \brief   formatEte called by TransmitFromNav. Function to convert estimated time enroute from milliseconds to minutes and seconds for the JSON object
	* \param ete The ETE (Estimated Time Enroute) in a millisecond format
	* \return  The ETE (Estimated Time Enroute) in a string format; eg. "2 min 58 sec"
	*/
	std::string formatEte(int ete) const;
	/**
	* \brief   formatPathString called by TransmitFromNav. Function to bracket a path_ vector correctly for the JSON object
	* \param path The path_ vector from NavigationData
	* \return  The path_ vector in a string format packaged correctly to insert in the fromNav.json file
	*/
	std::string formatPathString(std::vector<Coordinate> path) const;
};
