#pragma once
#include <JuceHeader.h>
#include "Editor/Editor.h"

class LFO: public juce::Component
{
public:
    LFO(juce::AudioProcessorValueTreeState& apvts);
    ~LFO();

    void paint(juce::Graphics &g) override;
    void resized() override;

private:
    Editor editor;
};
