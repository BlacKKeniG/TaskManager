#include <windows.h>
#include <iostream>
#include <future>
#include <thread>
#include <chrono>

#include "SimpleProgramTimer.h"
#include "TimeNowFunction.h"
#include "TaskManadger.h"

int main() {

	SimpleProgramTimer spt;

	std::atomic<bool> run{ true };
	
	std::cout << "[ " << timeNow() << " ]" << " main thread id: " << std::this_thread::get_id() << std::endl;

	try {
		mutex mtx_cout;
		TaskManadger taskManadger{ "../TasksConfig.json" };
		std::future<void> f{ std::async(std::launch::async, &TaskManadger::run, &taskManadger, std::ref(run), std::ref(mtx_cout)) };

		while (run) {
			if (GetAsyncKeyState(VK_ESCAPE)) {
				run.store(false, std::memory_order_relaxed);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
	catch (const std::exception& exñ) {
		std::cerr << exñ.what() << std::endl;
	}
	return 0;
}