#pragma once
#include <JuceHeader.h>
#include "../../Palette/Palette.h"

class Style2 : public juce::LookAndFeel_V4
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
        auto centreAngle = rotaryStartAngle + 0.5f * (rotaryEndAngle - rotaryStartAngle);

        // Knob
        g.setColour(Palette::control);
        g.fillEllipse(rx, ry, rw, rw);

        // Value arc
        juce::Path valueArc;

        if (sliderPos < 0.5f)
        {
            valueArc.addCentredArc(centreX, centreY, radius - 2.0f, radius - 2.0f, 0.0f, angle, centreAngle, true);
        }
        else if (sliderPos > 0.5f)
        {
            valueArc.addCentredArc(centreX, centreY, radius - 2.0f, radius - 2.0f, 0.0f, centreAngle, angle, true);
        }

        g.setColour(Palette::lithium);
        g.strokePath(valueArc, juce::PathStrokeType(3.0f,juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Centre marker
        juce::Path centreMarker;
        auto markerLength = radius * 0.20f;
        auto markerThickness = 2.0f;

        centreMarker.addRectangle(-markerThickness * 0.5f, -radius, markerThickness, markerLength);

        centreMarker.applyTransform(juce::AffineTransform::rotation(centreAngle).translated(centreX, centreY));

        g.setColour(Palette::borderLight);
        g.fillPath(centreMarker);

        // Pointer
        juce::Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 2.0f;

        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        g.setColour(Palette::text);
        g.fillPath(p);
    }
};