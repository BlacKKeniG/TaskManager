#include "TaskManager.h"

TaskManager::TaskManager(const string configFileName) :
	_taskConfiguration{ configFileName }, _QueueSimpleTasks{}, _queuesDelayTasks{}, _threads{} {
}

void TaskManager::run(const std::atomic<bool>& flag, mutex& mtxCoutForLog) {

	emplaseStartingDelayTasks(mtxCoutForLog);
	delayedTaskHandler(flag);
	priorityTasksHandler(flag, mtxCoutForLog);
	joinThreads();
}

void TaskManager::emplaseStartingDelayTasks(mutex& mtxCoutForLog) {
	for (unsigned i{ 0 }; i < _taskConfiguration._numOfDelayTask; ++i) {
		_queuesDelayTasks.push_back(Queue<QueueTask>{ _taskConfiguration._Task_D_Queue.at(i) });
		emplaseDelayTask(i, mtxCoutForLog);
	}
}

void TaskManager::emplaseDelayTask(const unsigned indexConfiguration, mutex& mtxCoutForLog) {

	_queuesDelayTasks.at(indexConfiguration)._container.emplace(_taskConfiguration._Task_D_Names.at(indexConfiguration),
		indexConfiguration, _taskConfiguration._Task_D_Queue.at(indexConfiguration), [&, indexConfiguration] {

			unsigned index{ indexConfiguration };

			unl_mutex unl_S1{ _mtxQueueS };
			_QueueSimpleTasks._container.emplace(_taskConfiguration._Task_S_Names.at(index), index, _taskConfiguration._Task_S_Queue.at(index),
				_taskConfiguration._Task_S_Priority.at(index), [&, index] {

					coutTaskLog(index, "running", mtxCoutForLog);

					std::this_thread::sleep_for(std::chrono::seconds(_taskConfiguration._Task_S_Delay.at(index)));

					coutTaskLog(index, "complited", mtxCoutForLog);

				});
			unl_S1.unlock();

			coutTaskLog(index, "pushed", mtxCoutForLog);

		}, _taskConfiguration._Task_D_Delay.at(indexConfiguration));

	coutTaskLog(indexConfiguration, "created", mtxCoutForLog);
}

void TaskManager::delayedTaskHandler(const std::atomic<bool>& flag) {
	for (auto& queue : _queuesDelayTasks) {
		_threads.push_back(thread{ [&] {
			while (flag) {
				if (!queue._container.empty()) {

					Task currentTask{ std::move(queue._container.front()) };
					queue._container.pop();
					currentTask();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		} });
	}
}

void TaskManager::priorityTasksHandler(const std::atomic<bool>& flag, mutex& mtxCoutForLog) {
	while (flag) {
		if (!_QueueSimpleTasks._container.empty()) {

			unl_mutex unl_S1{ _mtxQueueS };
			Task currentTask{ _QueueSimpleTasks._container.top()};
			_QueueSimpleTasks._container.pop();
			unl_S1.unlock();

			currentTask();

			emplaseDelayTask(currentTask._numberInConfigList, mtxCoutForLog);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void TaskManager::joinThreads() {
	for (auto& thread : _threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
}

void TaskManager::setTextColorByTaskIndex(const unsigned index) {

	HANDLE handel = GetStdHandle(STD_OUTPUT_HANDLE);

	if (index == 0) SetConsoleTextAttribute(handel, 1);
	else if (index == 1) SetConsoleTextAttribute(handel, 6);
	else if (index == 2) SetConsoleTextAttribute(handel, 7);
}

void TaskManager::coutTaskLog(const unsigned indexConfiguration, const string& status, mutex& mtx_cout) {

	unl_mutex unl_cout(mtx_cout);
	setTextColorByTaskIndex(indexConfiguration);
	if (indexConfiguration >= _taskConfiguration._numOfDelayTask || indexConfiguration < 0) {
		std::cout
			<< "[" << timeNow() << "] "
			<< " < " << "Config index: " << indexConfiguration << " not found!" << " > "
			<< std::endl;

	}
	if (status == "pushed") {
		std::cout
			<< " [ " << "thread id: " << std::this_thread::get_id() << " ] "
			<< " { " << timeNow() << " } "
			<< " < " << _taskConfiguration._Task_D_Names.at(indexConfiguration) << " > "
			<< " < " << "delay: " << _taskConfiguration._Task_D_Delay.at(indexConfiguration) << " > "
			<< " < " << _taskConfiguration._Task_S_Names.at(indexConfiguration) << " > "
			<< " < " << _taskConfiguration._Task_S_Queue.at(indexConfiguration) << " > "
			<< "pushed"
			<< std::endl;

	}
	else if (status == "created") {
		std::cout
			<< " [ " << "thread id: " << std::this_thread::get_id() << " ] "
			<< " { " << timeNow() << " } "
			<< " < " << _taskConfiguration._Task_D_Names.at(indexConfiguration) << " > "
			<< " < " << "delay: " << _taskConfiguration._Task_D_Delay.at(indexConfiguration) << " > "
			<< "created"
			<< std::endl;
	}
	else if (status == "running" || status == "complited") {
		std::cout
			<< " [ " << "thread id: " << std::this_thread::get_id() << " ] "
			<< " { " << timeNow() << " } "
			<< " < " << _taskConfiguration._Task_S_Queue.at(indexConfiguration) << " > "
			<< " < " << _taskConfiguration._Task_S_Names.at(indexConfiguration) << " > "
			<< " < " << "delay: " << _taskConfiguration._Task_S_Delay.at(indexConfiguration) << " > "
			<< " < " << "priority: " << _taskConfiguration._Task_S_Priority.at(indexConfiguration) << " > "
			<< status
			<< std::endl;
	}
	else {
		std::cout
			<< "[" << timeNow() << "] "
			<< " < " << "The status of the current task is undefined! Ñheck spelling!" << " > "
			<< std::endl;
	}
}