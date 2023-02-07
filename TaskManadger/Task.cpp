#include "Task.h";

Task::Task() : _name{}, _numberInConfigList{}, _queueName{}, _priority{}, _toDo{}, _delay{}, _executionTime{} {};

Task::Task(const string& name, const unsigned numberInConfigList, const string& queueName, const function<void()>& toDo) :
	_name{ name }, _numberInConfigList{ numberInConfigList }, _queueName{ queueName }, _toDo{ toDo }, _priority{}, _delay{}, _executionTime{} {};

Task::Task(const string& name, const unsigned numberInConfigList, const string& queueName, const unsigned priority, const function<void()>& toDo) :
	_name{ name }, _numberInConfigList{ numberInConfigList }, _queueName{ queueName }, _priority{ priority }, _toDo{ toDo }, _delay{}, _executionTime{} {};

Task::Task(const string& name, const unsigned numberInConfigList, const string& queueName, const function<void()>& toDo, const unsigned delay) :
	_name{ name }, _numberInConfigList{ numberInConfigList }, _queueName{ queueName }, _toDo{ toDo }, _delay{ delay }, _priority{}, _executionTime{} {};

Task::Task(const Task& other) {
	_name = other._name;
	_numberInConfigList = other._numberInConfigList;
	_queueName = other._queueName;
	_priority = other._priority;
	_toDo = other._toDo;
	_delay = other._delay;
	_executionTime = other._executionTime;
}

Task::Task(Task&& other) noexcept {

	_name = std::move(other._name);
	_queueName = std::move(other._queueName);
	_numberInConfigList = other._numberInConfigList;
	_priority = other._priority;
	_toDo = other._toDo;
	_delay = other._delay;
	_executionTime = other._executionTime;

	other._numberInConfigList = {};
	other._priority = {};
	other._toDo = {};
	other._delay = {};
	other._executionTime = {};
}

Task& Task::operator = (const Task& other) {

	_name = other._name;
	_queueName = other._queueName;
	_numberInConfigList = other._numberInConfigList;
	_priority = other._priority;
	_toDo = other._toDo;
	_delay = other._delay;
	_executionTime = other._executionTime;

	return *this;
}

Task& Task::operator = (Task&& other) noexcept {

	_name = std::move(other._name);
	_queueName = std::move(other._queueName);
	_numberInConfigList = other._numberInConfigList;
	_priority = other._priority;
	_toDo = other._toDo;
	_delay = other._delay;
	_executionTime = other._executionTime;

	other._numberInConfigList = {};
	other._priority = {};
	other._toDo = {};
	other._delay = {};
	other._executionTime = {};

	return *this;
}

void Task::operator()() {

	std::chrono::time_point<std::chrono::steady_clock> startWork{ std::chrono::steady_clock::now() };

	if (_delay) {
		std::this_thread::sleep_for(std::chrono::seconds(_delay));
	}
	if (_toDo) {
		_toDo();
	}

	std::chrono::duration<double> duration{ std::chrono::steady_clock::now() - startWork };
	_executionTime = static_cast<unsigned> (duration.count());
}