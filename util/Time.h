#pragma once

#include <chrono>

#include "../common/types.h"

namespace Core {

	class Time {

	public:

		static void update();

		static Real getRealTimeSinceStartup();
		static Real getTime();
		static Real getRealDeltaTime();
		static Real getDeltaTime();

  private:
    static Real timeScale;
		static Bool initialized;
		static unsigned long long startupTime;
		static std::chrono::high_resolution_clock::time_point _startupTime;
		static Real lastRecordedTime;
		static Real lastRecordedRealTime;
		static Real deltaTime;
		static Real realDeltaTime;

		static void initialize();

		Time();
		~Time();
	};
}
