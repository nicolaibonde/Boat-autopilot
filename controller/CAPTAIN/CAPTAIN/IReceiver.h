#pragma once
/**
 * \brief An interface that describes what should be present for a reciever.
 */
class IReceiver
{
public:
	/**
	 * \brief Destructor
	 */
	virtual ~IReceiver(){};
	/**
	 * \brief Recieve data from the UI , and call the apropriate function in the rest of CAPTAIN
	 */
	virtual void ReceiveToNav() = 0;
	/**
	 * \brief Recieve data about the active parameters from the UI, and tell other parts of CAPTAIN about their parameters
	 */
	virtual void ReceiveActiveParam() = 0;
};

