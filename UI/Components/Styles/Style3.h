#pragma once
#include <JuceHeader.h>
#include "../../Palette/Palette.h"

class Style3: public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(
        juce::Graphics& g,
        int x, int y,
        int width, int height,
        float sliderPos,
        const float rotaryStartAngle,
        const float rotaryEndAngle,
        juce::Slider& slider) override
    {
        auto radius = juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float) x + (float) width * 0.5f;
        auto centreY = (float) y + (float) height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;

        g.setColour(Palette::border);
        g.fillRoundedRectangle(rx, ry, rw, rw, 10);

        g.setColour(Palette::text);
        g.drawText(juce::String(slider.getValue()), rx, ry, rw, rw, juce::Justification::centred);
    }
};
