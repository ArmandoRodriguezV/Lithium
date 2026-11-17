#pragma once
#include <JuceHeader.h>
#include "../DSP/Osc.h"
#include "../DSP/ADSR.h"

class SVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound*) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;

    void prepare(double sampleRate, int samplesPerBlock);
    void setWaveframes1(const float* frame);
    void setWaveframes2(const float* frame);
    void setWaveframes3(const float* frame);

    void setGain(float osc1G, float osc2G, float osc3G);
    void setPan(float osc1P, float osc2P, float osc3P);
    void setTranspose(int val1, int val2, int val3);
    void setTune(int val1, int val2, int val3);

    void powerOscillators(bool osc1, bool osc2, bool osc3);

    void setAdsrValues(float a, float cA, float d, float cD, float s, float r, float cR);

private:
    Osc osc1;
    Osc osc2;
    Osc osc3;
    ADSR adsr;

    juce::AudioBuffer<float> voiceBuffer;
};