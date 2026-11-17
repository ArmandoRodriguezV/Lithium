#pragma once
#include <JuceHeader.h>
#include "Viewer.h"
#include "../Components/Styles/Style1.h"
#include "../Components/Styles/Style2.h"
#include "../Components/Styles/Style3.h"
#include "../Components/Sliders/Slider.h"

class Oscillator : public juce::Component
{
public:
    Oscillator(
        juce::AudioProcessorValueTreeState& apvts,
        const juce::String& powerId,
        const juce::String& gainId, const juce::String& panId,
        const juce::String& transposeId, const juce::String& tuneId,
        const juce::String& waveFrameId, const juce::String& waveSelectorId);
    ~Oscillator() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void setWaveFrame(const float* wF);
    void paintOverChildren(juce::Graphics &g) override;

private:
    void addItems(juce::Grid& grid);

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    Viewer viewer;

    juce::ToggleButton power;
    std::unique_ptr<ButtonAttachment> powerAttachment;

    Slider gain;
    Slider pan;
    Slider transpose;
    Slider tune;

    Style1 s1;
    Style2 s2;
    Style3 s3;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscillator)
};