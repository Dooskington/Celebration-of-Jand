#pragma once
#include "PCH.hpp"

class Timer
{
public:
    Timer();

    void Start();
    void Stop();
    void Pause();
    void UnPause();
    double GetTime();
    bool IsStarted();
    bool IsPaused();

private:
    double m_startTime;
    double m_pausedTime;
    bool m_isStarted;
    bool m_isPaused;
};