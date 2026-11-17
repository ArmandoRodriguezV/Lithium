#pragma once
#include <JuceHeader.h>

class Osc
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);

    void reset();
    void setFrequency(float frequency);
    void noteOn(float velocity);
    bool getState();

    void setActive(bool activate);
    void setGain(float g);
    void setPan(float p);
    void setTranspose(int val);
    void setTune(int val);

    void setWaveframe(const float* waveform);

private:
    // =================================================================================
    juce::Random rand;
    float getRandomRange(float minValue, float maxValue);

    float frequency = 440.0f;
    double sampleRate = 44100.0;

    float phase = 0.0f;
    float phaseIncrement = 0.0f;

    float amplitude = 1.0f;

    bool active = false;
    // =================================================================================

    const float PI = juce::MathConstants<float>::pi;
    const float TAU = juce::MathConstants<float>::twoPi;

    const float* waveframe = nullptr;

    // =================================================================================
    float gain = 1.0f;

    float pan = 0.0f;
    float leftGain = 0.7071f;
    float rightGain = 0.7071f;

    int transpose = 0;
    int tune = 0;
};