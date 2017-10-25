#pragma once
#include "IReceiver.h"
#include "INavigation.h"
#include "IAutopilot.h"
#include "json.hpp"
#include <boost/filesystem.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>


class JSONReceiver :
	public IReceiver
{
public:
	JSONReceiver(INavigation& navigation, IAutopilot & autopilot, const std::string saved_data_path);
	~JSONReceiver();

	void ReceiveToNav() override;
	void ReceiveActiveParam() override;

private:
	bool is_empty(std::ifstream& pFile);
	std::tuple<Task, TargetPosition, CoverageRectangle> func_ToTask(const nlohmann::json data);

private:
	boost::filesystem::path saved_data_path_;
	boost::filesystem::path active_param_path_;
	boost::filesystem::path to_nav_path_;
	nlohmann::json active_profile_;
	INavigation& navigation_;
	IAutopilot& autopilot_;
};

