#pragma once
#include <JuceHeader.h>
#include "../AutomationComponent.h"
#include "../Styles/Style1.h"

class Slider: public AutomationComponent
{
public:
    Slider(juce::AudioProcessorValueTreeState& apvts, const juce::String& id);
    ~Slider();

    void paint(juce::Graphics &g) override;
    void resized() override;

    void setSliderName(const juce::String &nm);

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    juce::Slider slider;
    juce::Label nameLabel;
    std::unique_ptr<SliderAttachment> attachment;

    Style1 style1;
};