#pragma once
#include <JuceHeader.h>

class ADSR
{
public:
    void prepare(double sR);
    void process(juce::AudioBuffer<float>& buffer, int startSample, int samplesPerBlock);

    void setAttack(const float a, const float c);
    void setDecay(const float d, const float c);
    void setSustain(const float s);
    void setRelease(const float r, const float c);

    void noteOn();
    void noteOff();

    bool isOff();

private:
    enum Stage {
        Attack,
        Decay,
        Sustain,
        Release,
        Off
    };

    double sampleRate = 44100.0;

    float attack = 0.5f;
    float attackCurve = 1.0f;
    int attackSamples = 0;

    float decay = 0.5f;
    float decayCurve = 1.0f;
    int decaySamples = 0;

    float sustainLevel = 1.0f;

    float release = 0.5f;
    float releaseCurve = 1.0f;
    int releaseSamples = 0;

    float releaseStartLevel = 0.0f;
    float currentLevel = 0.0f;

    int currentSample = 0;

    Stage stage = Attack;
};