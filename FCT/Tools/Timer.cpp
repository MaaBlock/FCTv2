#include "../headers.h"
namespace FCT {
    TimerShareData g_timerShareData;
    void AutoTicker(std::string name) {
        g_timerShareData.tick[name].tick();
    }
    float GetTps(std::string name) {
        return g_timerShareData.tick[name].tps();
    }
    float GetDuration(std::string name) {
        return g_timerShareData.duration[name];
    }
}