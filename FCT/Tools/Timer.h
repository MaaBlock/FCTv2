#pragma once
#include <string>
#include <chrono>
#include <map>
namespace FCT {

    class TimeCounter {
    public:
        TimeCounter() {
            m_duration = std::chrono::high_resolution_clock::duration(0);
            tick();
        }
        void tick() {
            auto now = std::chrono::high_resolution_clock::now();
            m_duration = now - m_lastTick;
            m_lastTick = now;
        }

        void begin() {
            tick();
        }

        void end() {
            tick();
        }

        float during() const {
            return std::chrono::duration<float>(m_duration).count();
        }
        float tps() const {
            return 1.0f / std::chrono::duration<float>(m_duration).count();
        }
    private:
        std::chrono::high_resolution_clock::time_point m_lastTick;
        std::chrono::high_resolution_clock::duration m_duration;
    };
    class TimerShareData {
    public:
        std::map<std::string, float> duration;
        std::map<std::string, TimeCounter> tick;
    };
    extern TimerShareData g_timerShareData;
    class AutoTimer {
    public:
        AutoTimer(std::string name) {
            m_name = name;
            m_counter.begin();
        }
        ~AutoTimer() {
            m_counter.end();
            g_timerShareData.duration[m_name] = m_counter.during();
        }
    private:
        TimeCounter m_counter;
        std::string m_name;
    };
    void AutoTicker(std::string name);
    float GetTps(std::string name);
    float GetDuration(std::string name);
    float GetTickDuration(std::string name);
}