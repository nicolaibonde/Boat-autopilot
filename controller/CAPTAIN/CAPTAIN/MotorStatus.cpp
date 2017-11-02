#include "MotorStatus.h"
#include "json.hpp"

MotorStatus::MotorStatus(const double percentage, const MotorType motor_type)
	{
		percentage_ = percentage;
		motor_type_ = motor_type;
	};

MotorStatus::~MotorStatus()
	{
		
	};

std::string MotorStatus::GetString()
{
		nlohmann::json motor = {
			{ "title_", motor_type_ ? ("Position") : ("Propulsion") },
			{ "items_",{
				{
					{ "data_",percentage_ },
					{ "title_",(motor_type_? ("Servo") : ("DC Motor"))},
					{ "unit_","%" },
					{ "color_","progress-bar-warning"}
				}
			}
			}
		};

		std::string result = motor.dump();

		return  result;
	}
