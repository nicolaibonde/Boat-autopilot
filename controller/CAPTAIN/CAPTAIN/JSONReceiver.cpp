#include "JSONReceiver.h"
#include "TargetPosition.h"
#include "CoverageRectangle.h"

JSONReceiver::JSONReceiver(INavigation& navigation, IAutopilot& autopilot, const std::string saved_data_path) : navigation_(navigation), autopilot_(autopilot), active_profile_({}), saved_data_path_(saved_data_path)
{
	//saved_data_path_ = boost::filesystem::current_path().parent_path().parent_path().parent_path().append("website/savedData");
	const boost::filesystem::directory_iterator end_itr;

	// cycle through the directory
	for (boost::filesystem::directory_iterator itr(saved_data_path_); itr != end_itr; ++itr)
	{
		// If it's not a directory.
		if (is_regular_file(itr->path())) {
			if(itr->path().filename() == "activeParam.json")
			{
				active_param_path_ = itr->path();
			}
			if(itr->path().filename() == "toNav.json")
			{
				to_nav_path_ = itr->path();
			}
		}
	}

}

JSONReceiver::~JSONReceiver()
{
}

void JSONReceiver::ReceiveToNav()
{
	std::ifstream file_stream(to_nav_path_.string());
	nlohmann::json j;
	if(!is_empty(file_stream))
	{
		file_stream >> j;
	}else
	{
		j = { { "func_","none"} };
	}


	std::tuple<Task, TargetPosition, CoverageRectangle> task = func_ToTask(j);

	if (std::get<0>(task) == CalcP2P)
		navigation_.PerformTask(std::get<0>(task), std::get<1>(task)); //Ignoring the 3 element in the tuple because it is not needed and is null in this case
	else if (std::get<0>(task) == CalcCoverageRectangle)
		navigation_.PerformTask(std::get<0>(task), std::get<2>(task)); //Ignoring the 2 element int the tuple the same as before
	else
		navigation_.PerformTask(std::get<0>(task));
	
}

void JSONReceiver::ReceiveActiveParam()
{
	std::ifstream file_stream(active_param_path_.string());
	if (!is_empty(file_stream))
	{
		file_stream >> active_profile_;

		int counter = 0;
		float P = -1;
		float I = -1;
		float D = -1;
		float tool_width = -1;
		if (active_profile_.find("parameters_") != active_profile_.end() &&
			active_profile_.find("parameter_names_") != active_profile_.end()) {
			if(active_profile_["parameters_"].size() == active_profile_["parameter_names_"].size())
				try
				{
				for (nlohmann::json::iterator it = active_profile_["parameter_names_"].begin(); it != active_profile_["parameter_names_"].end(); ++it)
				{
					if (*it == "P")
					{
						P = active_profile_["parameters_"][counter];
					}
					if (*it == "I")
					{
						I = active_profile_["parameters_"][counter];
					}
					if (*it == "D")
					{
						D = active_profile_["parameters_"][counter];
					}
					if (*it == "Tool Width")
					{
						tool_width = active_profile_["parameters_"][counter];
					}
					counter++;
				}
				}
				catch (std::domain_error)
				{
				}
		}
		if (P != -1 && I != -1 && D != -1 && tool_width != -1)
		{
			autopilot_.SetParameters(P, I, D);
			navigation_.SetParameters(tool_width);
		}
	}
	
}

bool JSONReceiver::is_empty(std::ifstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}

std::tuple<Task, TargetPosition, CoverageRectangle> JSONReceiver::func_ToTask(const nlohmann::json data)
{
	const nlohmann::json func = data.find("func_").value();
	if (func == "calcP2P")
	{
		TargetPosition target = TargetPosition();
		if (data.find("target_position_") != data.end()) {
			if (data["target_position_"].find("latitude_") != data["target_position_"].end() &&
				data["target_position_"].find("longitude_") != data["target_position_"].end())
			{
				//If there is a logitude and latitude
				try
				{
					target = TargetPosition(Coordinate(data["target_position_"]["latitude_"], data["target_position_"]["longitude_"]));
				}
				catch (std::domain_error)
				{
				}
				
			}
		}
		return std::tuple<Task, TargetPosition, CoverageRectangle>(CalcP2P, target, CoverageRectangle());
	}
	if (func == "calcCoverage")
	{
		CoverageRectangle rectangle = CoverageRectangle();
		if (data.find("coverage_rectangle_") != data.end())
		{
			nlohmann::json cr = data["coverage_rectangle_"];
			if (cr.find("start_coord_") != cr.end() &&
				cr.find("end_coord_") != cr.end())
			{
				nlohmann::json sc = cr["start_coord_"];
				nlohmann::json ec = cr["end_coord_"];
				if (sc.find("latitude_") != sc.end() &&
					sc.find("longitude_") != sc.end() &&
					ec.find("latitude_") != ec.end() &&
					ec.find("longitude_") != ec.end())
				{
					//If there is a logitude and latitude for both start and end coord
					try
					{
						rectangle = CoverageRectangle(
							Coordinate(data["coverage_rectangle_"]["start_coord_"]["latitude_"],
								data["coverage_rectangle_"]["start_coord_"]["longitude_"]),
							Coordinate(data["coverage_rectangle_"]["end_coord_"]["latitude_"],
								data["coverage_rectangle_"]["end_coord_"]["longitude_"])
						);
					}
					catch (std::domain_error){
					
					}
				}
			}
		}

		return std::tuple<Task, TargetPosition, CoverageRectangle>(CalcCoverageRectangle, TargetPosition(), rectangle);
	}
	if (func == "start")
	{
		return std::tuple<Task, TargetPosition, CoverageRectangle>(Start, TargetPosition(), CoverageRectangle());
	}
	if (func == "stop")
	{
		return std::tuple<Task, TargetPosition, CoverageRectangle>(Stop, TargetPosition(), CoverageRectangle());
	}
	return std::tuple<Task, TargetPosition, CoverageRectangle>(None, TargetPosition(), CoverageRectangle());

}