#include "Slider.h"
#include "../../Palette/Palette.h"

Slider::Slider(juce::AudioProcessorValueTreeState& apvts, const juce::String& id)
{
    addAndMakeVisible(slider);
    addAndMakeVisible(nameLabel);

    nameLabel.setJustificationType(juce::Justification::centred);
    nameLabel.setColour(juce::Label::textColourId, Palette::text);
    nameLabel.setInterceptsMouseClicks(false, false);

    attachment = std::make_unique<SliderAttachment>(apvts, id, slider);
}

Slider::~Slider()
{
    slider.setLookAndFeel(nullptr);
}

void Slider::paint(juce::Graphics &g)
{
}

void Slider::resized()
{
    auto w = getWidth();
    auto h = getHeight();

    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, 0.0f, 0.0f, false);
    slider.setLookAndFeel(&getLookAndFeel());
    slider.setBounds(0, 0, w, h * 0.78f);

    nameLabel.setBounds(0, h * 0.75f, w, h * 0.25f);
}

void Slider::setSliderName(const juce::String& nm)
{
    nameLabel.setText(nm, juce::dontSendNotification);
}

