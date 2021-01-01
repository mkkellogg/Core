#pragma once

#include "../common/types.h"
#include "Time.h"

namespace Core {

    class Profiler {
    public:

        class SingleFunction {
        public:

            static void quickSinglePassStart(Real startTime) {
                if (quickSinglePassProfCount % 100 == 0) std::cerr << "==========================================" << std::endl;
                quickSinglePassStartTime = startTime;
                quickSinglePassLastTime = Time::getRealTimeSinceStartup();
                quickSinglePassTotalFrameTime = 0.0f;
            }

            static void quickSinglePassSection(const std::string& label) {
                Real currentTime = Time::getRealTimeSinceStartup();
                Real partTime = currentTime - quickSinglePassLastTime;
                quickSinglePassTotalFrameTime += partTime;
                if (quickSinglePassProfCount % 100 == 0) std::cerr << label << std::to_string(partTime) << std::endl;
                quickSinglePassLastTime = currentTime;
            }

            static void quickSinglePassEnd(Bool displayOutput) {
                Real currentTime = Time::getRealTimeSinceStartup();
                if ((quickSinglePassProfCount % 100 == 0)&& displayOutput) std::cerr << "Total frame time: " << std::to_string(quickSinglePassTotalFrameTime) << std::endl;
                if (currentTime >= quickSinglePassStartTime) {
                    quickSinglePassProfCount++;
                }
            }

        private:
            static Real quickSinglePassStartTime;
            static UInt32 quickSinglePassProfCount;
            static Real quickSinglePassTotalFrameTime;
            static Real quickSinglePassLastTime;
        };
    };
}