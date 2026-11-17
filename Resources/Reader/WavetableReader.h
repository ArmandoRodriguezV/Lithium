#pragma once
#include <JuceHeader.h>

class WavetableReader
{
public:
    static constexpr int tableSize = 1024;

    bool load(const void* data, size_t size);
    const std::vector<std::vector<float>>& getTables() const;
    int getTotalTables() const;
    const std::vector<float>& getTable(int index) const;

    static void setWaveFrame1(const float* wF) { waveFrame1 = wF; };
    static void setWaveFrame2(const float* wF) { waveFrame2 = wF; };
    static void setWaveFrame3(const float* wF) { waveFrame3 = wF; };

    static const float* getWaveFrame1() { return waveFrame1; }
    static const float* getWaveFrame2() { return waveFrame2; }
    static const float* getWaveFrame3() { return waveFrame3; }

private:
    std::vector<std::vector<float>> tables;

    static const float* waveFrame1;
    static const float* waveFrame2;
    static const float* waveFrame3;
};