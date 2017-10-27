#include "JSONReceiver.h"
#include "TargetPosition.h"
#include "CoverageRectangle.h"

JSONReceiver::JSONReceiver(INavigation& navigation, IAutopilot& autopilot, const std::string saved_data_path) : saved_data_path_(saved_data_path), active_profile_({}), navigation_(navigation), autopilot_(autopilot)
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
	std::ifstream file_stream(to_nav_path_.string());	//Settingup a file stream at the path of the toNav.JSON file
	nlohmann::json toNav;								//Creating a json object to hold the toNav object
	if(!is_empty(file_stream))
	{
		file_stream >> toNav;							//If the file is not empty load the file into the json object
	}else
	{
		toNav = { { "func_","none"} };					//Else create a default object
	}


	//Get the TargetPosition or CoverageRectangle extracted from the json object, and error checked.
	std::tuple<Task, TargetPosition, CoverageRectangle> task = func_ToTask(toNav);


	if (std::get<0>(task) == CalcP2P)					//If the task is CalcP2P then only use targetPosition
		navigation_.PerformTask(std::get<0>(task), std::get<1>(task)); //Ignoring the 3 element in the tuple because it is not needed and is null in this case
	
	else if (std::get<0>(task) == CalcCoverageRectangle)//If the task is CalcCoverage then use the CoverageRectangle
		navigation_.PerformTask(std::get<0>(task), std::get<2>(task)); //Ignoring the 2 element int the tuple the same as before
	else {												//Else dont send either CoverageRectangle nor targetPosition
		navigation_.PerformTask(std::get<0>(task));
	}
}

void JSONReceiver::ReceiveActiveParam()
{
	std::ifstream file_stream(active_param_path_.string()); //Setting up a file stream at the path of the activeParam.JSON file

	if (!is_empty(file_stream))								//If the file is not empty continue or else just return with noting done
	{
		file_stream >> active_profile_;						//read the file into the member 

		int counter = 0; // Used to keep parameters_ and parameter_names_ read aligned.

		float P = -1;				//temporarily holds the P value read from the activeParam file, it is -1, used as an illeagal value
		float I = -1;				//temporarily holds the I value read from the activeParam file, it is -1, used as an illeagal value
		float D = -1;				//temporarily holds the D value read from the activeParam file, it is -1, used as an illeagal value
		float tool_width = -1;		//temporarily holds the tool_width value read from the activeParam file, it is -1, used as an illeagal value

		//Check if both paramters and parameter_names_ is in the file
		if (active_profile_.find("parameters_") != active_profile_.end() && 
			active_profile_.find("parameter_names_") != active_profile_.end()) {
			
			//Check if parameters_ and parameter_names_ are the same size
			if(active_profile_["parameters_"].size() == active_profile_["parameter_names_"].size())
				
				//catch on a domain error, which indicates that json object value is not as expected.
				try
				{
					//run through active_profiles and find the 4 names we are intrested in and getting there values in the other list
					for (nlohmann::json::iterator it = active_profile_["parameter_names_"].begin(); it != active_profile_["parameter_names_"].end(); ++it)
					{
						//if the value of the active_profile_ item we have reached is "P"
						if (*it == "P")
						{	
							//Then save the corresponding value from parameters_ into the correct local variable
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
						counter++; //Keeping counter up to date so we dont get drift
					}
				}
				catch (std::domain_error)
				{
					//If there is a domain error then we the variables that have not yet been set stay -1 to indicate an error
				}
		}
		//If no errors has occured then continue
		if (P != -1 && I != -1 && D != -1 && tool_width != -1)
		{	
			//call SetParameters in the autopilot and navigation via interfaces with the appropriate arguments.
			autopilot_.SetParameters(P, I, D);
			navigation_.SetParameters(tool_width);
		}
	}
	
}

//Used to check if a file is empty
bool JSONReceiver::is_empty(std::ifstream& file_stream)
{
	return file_stream.peek() == std::ifstream::traits_type::eof(); //If the first read of the file is the same as end of file then its empty
}

//A helper function that parses the json object given and returns a Task and TargetPosition Or CoverageRectangle
std::tuple<Task, TargetPosition, CoverageRectangle> JSONReceiver::func_ToTask(const nlohmann::json data)
{

	const nlohmann::json func = data.find("func_").value();		//Get the value of the func_ tag
	
	//If the func tag is calcP2P
	if (func == "calcP2P")
	{
		//Create an empty TargetPosition
		TargetPosition target = TargetPosition();

		//Check if target_position_ exists
		if (data.find("target_position_") != data.end()) {

			//Check if latitude and logitude exists within target_position
			if (data["target_position_"].find("latitude_") != data["target_position_"].end() &&
				data["target_position_"].find("longitude_") != data["target_position_"].end())
			{
				//If there is a logitude and latitude then we catch domain_errors to make sure 
				//there is the right data type in latitude and logitude ei. float or double
				try
				{
					//If everything is good then we create a TargetPosition object that we can then return which is not the default.
					target = TargetPosition(Coordinate(data["target_position_"]["latitude_"], data["target_position_"]["longitude_"]));
				}
				catch (std::domain_error)
				{
				}
			}
		}
		//No we return a tubple consisting of the Task, the just parsed target and an empty CoverageRectangle() to signify that it should not be used.
		return std::tuple<Task, TargetPosition, CoverageRectangle>(CalcP2P, target, CoverageRectangle());
	}
	//Check if the func_ tag is calcCoverage
	if (func == "calcCoverage")
	{
		CoverageRectangle rectangle = CoverageRectangle();		//Create an empty CoverageRectangle
		//Check if the coverage_rectangle_ exists in the data given.
		if (data.find("coverage_rectangle_") != data.end())
		{
			nlohmann::json cr = data["coverage_rectangle_"]; //Used to minimize repetision of coverage_rectangle_
		
			//Check if start_coord and end_coord_ exsists
			if (cr.find("start_coord_") != cr.end() &&
				cr.find("end_coord_") != cr.end())
			{
				nlohmann::json sc = cr["start_coord_"]; //Used to minimize repetisition of start_coord_
				nlohmann::json ec = cr["end_coord_"];	//Used to minimize repetisition of end_coord_
				
				//Check if there is a latitude_ and a longitude in start_coord and end_coord_
				if (sc.find("latitude_") != sc.end() &&
					sc.find("longitude_") != sc.end() &&
					ec.find("latitude_") != ec.end() &&
					ec.find("longitude_") != ec.end())
				{
					//If there is a logitude and latitude for both start_coord_ and end_coord_
					//then catch on domain errors to check if the value read matches the value expected
					try
					{
						//Create the CoverageRectangle object with the data just checked.
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

		//Return the Task and rectangle and a default TargetPosition(), to indicate it should not be used.
		return std::tuple<Task, TargetPosition, CoverageRectangle>(CalcCoverageRectangle, TargetPosition(), rectangle);
	}
	//if func_ is start
	if (func == "start")
	{
		//we return the Task Start and a default TargetPosition and CoverageRectangle
		return std::tuple<Task, TargetPosition, CoverageRectangle>(Start, TargetPosition(), CoverageRectangle());
	}
	if (func == "stop")
	{
		//we return the Task Stop and a default TargetPosition and CoverageRectangle
		return std::tuple<Task, TargetPosition, CoverageRectangle>(Stop, TargetPosition(), CoverageRectangle());
	}
	//if no task is found we return the Task None and a default TargetPosition and CoverageRectangle
	return std::tuple<Task, TargetPosition, CoverageRectangle>(None, TargetPosition(), CoverageRectangle());

}