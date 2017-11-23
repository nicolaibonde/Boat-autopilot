#pragma once
#include "IReceiver.h"
#include "INavigation.h"
#include "IAutopilot.h"
#include "json.hpp"
#include <boost/filesystem.hpp>

/**
 * \brief An implementaion of the IReciever interface where the input is in JSON
 */
class JSONReceiver :
	public IReceiver
{
public:
	/**
	 * \brief Explicit constructor
	 * \param navigation A reference to the navigation so it can be told when new data is avaliable, and to the its parameters
	 * \param autopilot A reference to the autopilot so it can be told about it paramters
	 * \param saved_data_path Where is the toNav.JSON and activeParam.JSON located
	 */
	JSONReceiver(INavigation& navigation, IAutopilot & autopilot, const std::string saved_data_path);
	/**
	 * \brief Destructor
	 */
	~JSONReceiver();

	/**
	 * \brief Recieves the data in the toNav.JSON file
	 * \details This function is resposible for updating the rest of the system about data from the user.
	 */
	void ReceiveToNav() override;
	/**
	 * \brief Recieves the data in the activeParam.JSON file
	 * \details This function is resposible for updating the navigation and autopilot parameters
	 */
	void ReceiveActiveParam() override;

private:
	/**
	 * \brief Determines if a file is empty
	 * \param pFile The file to check
	 * \return Whether or not a file is empty
	 */
	bool is_empty(std::ifstream& pFile);
	/**
	 * \brief A helper function to generate the needed object and make sure the right combination is returned
	 * \param data A JSON object containing either a TargetPostion, CoverageRectangle JSON object, or it might be empty
	 * \return Returns a Task which is the task from the JSON object if the task is a TargetPosition or a CoverageRectangle it returns one of those objects aswell, if not present it returns an empty object
	 */
	std::tuple<Task, TargetPosition, CoverageRectangle> func_ToTask(const nlohmann::json data);

private:
	/**
	 * \brief The path where the toNav.JSON and activeParam.JSON is located
	 */
	boost::filesystem::path saved_data_path_;
	/**
	 * \brief The file path of activeParam.JSON
	 */
	boost::filesystem::path active_param_path_;
	/**
	 * \brief The file path of toNav.JSON
	 */
	boost::filesystem::path to_nav_path_;
	/**
	 * \brief The JSON object contain the current parameters set by the user
	 */
	nlohmann::json active_profile_;
	/**
	 * \brief A refrence to a navigation object, used to perform tasks and set parameters
	 */
	INavigation& navigation_;

	/**
	 * \brief A reference to the autopilot object, used to set parameters
	 */
	IAutopilot& autopilot_;
};

