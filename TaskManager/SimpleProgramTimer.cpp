#include "SimpleProgramTimer.h"

#include <iostream>

SimpleProgramTimer::SimpleProgramTimer() : startWork{ std::chrono::steady_clock::now() }
, endWork{} { }

SimpleProgramTimer::~SimpleProgramTimer() {
	endWork = std::chrono::steady_clock::now();
	std::chrono::duration<double> duration{ std::chrono::steady_clock::now() - startWork };
	std::cout << "Duration work is: " << duration.count() << " sec" << std::endl;
}