//
// Created by Mark Kellogg on 1/18/18.
//

#ifndef MPM_TIME_H
#define MPM_TIME_H


#include <chrono>

#include "../common/types.h"

namespace Core {

	class Time {
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

	public:

		static void update();

		static Real getRealTimeSinceStartup();
		static Real getTime();
		static Real getRealDeltaTime();
		static Real getDeltaTime();

	};
}


#endif //MPM_TIME_H
