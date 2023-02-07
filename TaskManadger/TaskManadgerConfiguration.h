#pragma once

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

using std::string;
using std::vector;

	class TaskManadgerConfiguration {
	public:
		explicit TaskManadgerConfiguration(const string& jsonFileName);
		~TaskManadgerConfiguration() = default;
	
		friend class TaskManadger;
	protected:
	
		unsigned _numOfDelayTask;
		unsigned _numOfSimpleTask;

		vector<string> _Task_D_Names;
		vector<string> _Task_D_Queue;
		vector<unsigned> _Task_D_Delay;
	
		vector<string> _Task_S_Names;
		vector<unsigned> _Task_S_Delay;
		vector<string> _Task_S_Queue;
		vector<unsigned> _Task_S_Priority;
	
	private:
		void initFromJson(const string& jsonFileName);
	};
