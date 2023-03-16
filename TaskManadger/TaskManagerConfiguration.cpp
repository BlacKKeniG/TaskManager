#include "TaskManagerConfiguration.h"

TaskManagerConfiguration::TaskManagerConfiguration(const string& jsonFileName) : _numOfDelayTask{}, _numOfSimpleTask{},
_Task_D_Names{}, _Task_D_Queue{}, _Task_D_Delay{},
_Task_S_Names{}, _Task_S_Delay{}, _Task_S_Queue{}, _Task_S_Priority{} {
	initFromJson(jsonFileName);
}

void TaskManagerConfiguration::initFromJson(const string& jsonFileName) {
	boost::property_tree::ptree root;
	boost::property_tree::read_json(jsonFileName, root);

	boost::property_tree::ptree& TasksD_Config{ root.get_child("TasksD_Config") };
	boost::property_tree::ptree& TasksS_Config{ root.get_child("TasksS_Config") };
	_numOfDelayTask = TasksD_Config.size();
	_numOfSimpleTask = TasksS_Config.size();

	unsigned taskIndex{ 1 };
	for (boost::property_tree::ptree::const_iterator itD = TasksD_Config.begin(), itS = TasksS_Config.begin();
		itD != TasksD_Config.end(), itS != TasksS_Config.end(); ++itD, ++itS) {

		_Task_D_Names.push_back(itD->second.get<string>("task" + std::to_string(taskIndex) + "_D_name"));
		_Task_D_Queue.push_back(itD->second.get<string>("task" + std::to_string(taskIndex) + "_D_queue"));
		_Task_D_Delay.push_back(itD->second.get<unsigned>("task" + std::to_string(taskIndex) + "_D_delay"));

		_Task_S_Names.push_back(itS->second.get<string>("task" + std::to_string(taskIndex) + "_S_name"));
		_Task_S_Delay.push_back(itS->second.get<unsigned>("task" + std::to_string(taskIndex) + "_S_delay"));
		_Task_S_Queue.push_back(itS->second.get<string>("task" + std::to_string(taskIndex) + "_S_queue"));
		_Task_S_Priority.push_back(itS->second.get<unsigned>("task" + std::to_string(taskIndex) + "_S_priority"));

		++taskIndex;
	}
}