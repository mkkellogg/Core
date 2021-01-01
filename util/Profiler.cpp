#include "Profiler.h"

namespace Core {

    Real Profiler::SingleFunction::quickSinglePassStartTime;
    UInt32 Profiler::SingleFunction::quickSinglePassProfCount = 0;
    Real Profiler::SingleFunction::quickSinglePassTotalFrameTime;
    Real Profiler::SingleFunction::quickSinglePassLastTime;

}