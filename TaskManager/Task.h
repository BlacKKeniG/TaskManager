#pragma once

#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <functional>
#include <chrono>
#include <thread>

using std::string;
using std::function;
using std::thread;

class Task {

public:
	string _name;
	unsigned _numberInConfigList{};
	string _queueName;
	unsigned _priority;
	function<void()> _toDo;
	unsigned _delay;
	unsigned _executionTime{};

	explicit Task();
	explicit Task(const string& name, const unsigned numberInConfigList, const string& queueName, const function<void()>& toDo);
	explicit Task(const string& name, const unsigned numberInConfigList, const string& queueName, const unsigned priority, const function<void()>& toDo);
	explicit Task(const string& name, const unsigned numberInConfigList, const string& queueName, const function<void()>& toDo, const unsigned delay);

	Task(const Task& other);
	Task(Task&& other) noexcept;

	~Task() = default;

	Task& operator = (const Task& other);
	Task& operator = (Task&& other) noexcept;

	void operator()();
};