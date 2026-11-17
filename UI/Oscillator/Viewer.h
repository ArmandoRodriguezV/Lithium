#pragma once
#include <JuceHeader.h>
#include "../../Resources/Library/WavetablesLibrary.h"

class Viewer : public juce::Component
{
public:
    Viewer(
        juce::AudioProcessorValueTreeState& apvts,
        juce::String waveFrameId,
        juce::String waveSelectorId);
    ~Viewer() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setWaveFrame(const float* wF);

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    juce::AudioProcessorValueTreeState& apvts;
    juce::String waveSelectorId;

    juce::Slider waveFrameSlider;
    std::unique_ptr<SliderAttachment> waveFrameAttachment;

    juce::TextButton wavetableMenuBtn;

    const float sliderSize = 20.0f;
    const float selectorHeight = 25.0f;

    const float* waveFrame = nullptr;

    void createWaveTableMenu();
    void updateWaveTableButton();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Viewer)
};