#pragma once
#include <JuceHeader.h>
#include "../../Palette/Palette.h"

class Style1 : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(
        juce::Graphics& g,
        int x, int y,
        int width, int height,
        float sliderPos,
        const float rotaryStartAngle,
        const float rotaryEndAngle,
        juce::Slider&) override
    {
        auto radius = (float) juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float) x + (float) width * 0.5f;
        auto centreY = (float) y + (float) height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // Knob
        g.setColour(Palette::control);
        g.fillEllipse(rx, ry, rw, rw);

        // Knob outline
        g.setColour(Palette::border);
        g.drawEllipse(rx, ry, rw, rw, 1.0f);

        // Value arc
        juce::Path valueArc;
        valueArc.addCentredArc(centreX, centreY, radius - 2.0f, radius - 2.0f, 0.0f, rotaryStartAngle, angle, true);

        g.setColour(Palette::lithium);
        g.strokePath(valueArc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Pointer
        juce::Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 2.0f;

        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);

        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        g.setColour(Palette::text);
        g.fillPath(p);
    }

    void drawToggleButton(
        juce::Graphics& g,
        juce::ToggleButton& button,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        const auto bounds = button.getLocalBounds().toFloat();
        const bool toggled = button.getToggleState();

        const float switchWidth = bounds.getWidth();
        const float switchHeight = bounds.getHeight();

        const auto switchBounds = juce::Rectangle<float>(0.0f, (bounds.getHeight() - switchHeight) * 0.5f, switchWidth, switchHeight);
        g.setColour(toggled ? Palette::lithium : Palette::lithiumDark);
        g.fillRoundedRectangle(switchBounds, switchHeight * 0.5f);

        const float thumbSize = switchHeight - 6.0f;
        const float thumbX = toggled ? switchBounds.getRight() - thumbSize - 3.0f : switchBounds.getX() + 3.0f;

        g.setColour(Palette::lithiumLight);
        g.fillEllipse(thumbX, switchBounds.getY() + 3.0f, thumbSize, thumbSize);
    }
};













