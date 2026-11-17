#pragma once
#include <JuceHeader.h>

enum class ThumbType
{
    Simple,
    Curve
};

class Thumb: public juce::Component
{
public:
    void paint(juce::Graphics &g) override;
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;

    void setLimit(juce::Rectangle<float> newLimit);

    void setType(ThumbType newType);
    juce::Point<float> getCenterPosition() const;
    void setCenterPosition(juce::Point<float> newPosition);

    std::function<void()> onPositionChanged;

private:
    juce::ComponentDragger dragger;
    juce::Point<float> position;



    juce::Rectangle<float> limit;
    ThumbType type = ThumbType::Simple;
};