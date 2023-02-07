#pragma once

#include <iostream>
#include <windows.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <queue>

#include "TimeNowFunction.h"
#include "Task.h"
#include "Queue.h"
#include "TaskManadgerConfiguration.h"

using std::string;
using std::thread;
using std::mutex;
using unl_mutex = std::unique_lock<mutex>;

struct CompareTask {
	bool operator() (Task& t1, Task& t2) {
		return t2._priority < t1._priority;
	}
};
using QueueTask = std::queue<Task>;
using PriorityQueueTask = std::priority_queue<Task, std::deque<Task>, CompareTask>;
using Queues = std::vector<Queue<QueueTask>>;
using Threads = std::vector<std::thread>;

class TaskManadger
{
public:
	explicit TaskManadger(const string configFileName);

	~TaskManadger() = default;

	void run(const std::atomic<bool>& flag, mutex& mtxCoutForLog);

private:
	TaskManadgerConfiguration _taskConfiguration;
	Queues _queuesDelayTasks;
	Queue<PriorityQueueTask> _QueueSimpleTasks;
	mutex _mtxQueueS;
	Threads _threads;

	void emplaseStartingDelayTasks(mutex& mtxCoutForLog);
	void emplaseDelayTask(const unsigned indexConfiguration, mutex& mtxCoutForLog);
	void delayedTaskHandler(const std::atomic<bool>& flag);
	void priorityTasksHandler(const std::atomic<bool>& flag, mutex& mtxCoutForLog);
	void joinThreads();

	void setTextColorByTaskIndex(const unsigned index);

	void coutTaskLog(const unsigned indexConfiguration, const string& status, mutex& mtx_cout);
};