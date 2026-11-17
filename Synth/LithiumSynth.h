#pragma once
#include <JuceHeader.h>
#include "SSound.h"
#include "SVoice.h"
#include "../Resources/Reader/WavetableReader.h"
#include "../Resources/Library/WavetablesLibrary.h"

class LithiumSynth {
public:
    LithiumSynth();
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer, const juce::MidiBuffer& midiMessages);

    void setOscGain(float osc1G, float osc2G, float osc3G);
    void setOscPan(float osc1P, float osc2P, float osc3P);
    void setTranspose(int val1, int val2, int val3);
    void setTune(int val1, int val2, int val3);

    void setWaveFrameOsc1(float val);
    void setWaveFrameOsc2(float val);
    void setWaveFrameOsc3(float val);

    void loadWaveformIntoOscillator1(int osc1);
    void loadWaveformIntoOscillator2(int osc1);
    void loadWaveformIntoOscillator3(int osc1);

    void powerOscillators(bool osc1, bool osc2, bool osc3) const;
    void setAdsrValues(float a, float cA, float d, float cD, float s, float r, float cR) const;

private:
    std::vector<SVoice*> voices;
    juce::Synthesiser synth;

    WavetableReader reader1;
    WavetableReader reader2;
    WavetableReader reader3;

    int readerWf1 = 0;
    int readerWf2 = 0;
    int readerWf3 = 0;
};