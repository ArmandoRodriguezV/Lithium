#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/Oscillator/Oscillator.h"
#include "UI/ADSR/EnvelopeADSR.h"
#include "UI/LFO/LFO.h"

class LithiumAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    LithiumAudioProcessorEditor (LithiumAudioProcessor&);
    ~LithiumAudioProcessorEditor() override;

    void timerCallback() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    LithiumAudioProcessor& audioProcessor;

    Oscillator osc1;
    Oscillator osc2;
    Oscillator osc3;

    EnvelopeADSR envelopeAdsr;
    LFO lfo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LithiumAudioProcessorEditor)
};
