#pragma once
#include <JuceHeader.h>
#include "GraphAdsr.h"
#include "../Components/Sliders/Slider.h"
#include "../Components/Styles/Style1.h"

class EnvelopeADSR: public juce::Component
{
public:
    EnvelopeADSR(juce::AudioProcessorValueTreeState& apvts);
    ~EnvelopeADSR();

    void paint(juce::Graphics &g) override;
    void resized() override;

private:
    Slider attack, decay, sustain, release;
    Style1 style1;

    void positionedToGrid(juce::Grid& g);

    GraphAdsr graphAdsr;
};