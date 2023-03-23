#pragma once
#include <chrono>

using namespace std;

class MyTimer
{
private:
    std::chrono::steady_clock::time_point st;
    std::chrono::steady_clock::time_point et;

public:
    void start()
    {
        this->st = std::chrono::steady_clock::now();
    }

    void end()
    {
        this->et = std::chrono::steady_clock::now();
    }

    /**
     * @return 밀리세컨드 단위의 시간 간격 (long long)
    */
    auto getTimeLapse() {
        auto timelapse = std::chrono::duration_cast<std::chrono::milliseconds>(this->et - this->st).count();
        return timelapse;
    }
};

// auto time = std::chrono::high_resolution_clock::now();