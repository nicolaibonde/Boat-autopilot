#pragma once

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

class ITransmitter
{
public:
	virtual ~ITransmitter() {};
	/**
	* \brief   Navigation acquisition called by Navigation constructor with an INavigation this-pointer to link Navigation and JSONTransmitter \n
	* correctly, since mutual references are not possible.
	* \param navigation A this-pointer to Navigation object
	*/
	virtual void NavAcquisition(INavigation* navigation) = 0;
	/**
	* \brief   TransmitFromNavigation
	* \param timestamp The last timestamp set in the Navigation
	* \details Called by Navigation perform task in all three overloads with a timestamp string. \n
	* This functionality has the responsibility of collecting all relevant data from the necessary classes in the system, package \n
	* them in accordance with the fromNav.json protocol, and overwrite the json file with the new data. \n
	* The classes that information is pulled from are DCMotor, Servo, GPS, and Navigation. \n
	*/
	virtual void TransmitFromNav(std::string const filepath) = 0;
};
