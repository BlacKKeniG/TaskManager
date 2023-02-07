#pragma once

#include <chrono>

struct SimpleProgramTimer
{
public:
	SimpleProgramTimer();
	~SimpleProgramTimer();
private:
	std::chrono::time_point<std::chrono::steady_clock> startWork, endWork;
};