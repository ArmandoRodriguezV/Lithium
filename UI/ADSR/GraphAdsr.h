#pragma once
#include <JuceHeader.h>
#include "../Components/Thumb/Thumb.h"

class GraphAdsr : public juce::Component, public juce::Timer
{
public:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    GraphAdsr(juce::AudioProcessorValueTreeState& apvts);
    ~GraphAdsr() override;
    void timerCallback() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void syncCurveThumb(Thumb& curveThumb, juce::Point<float> start, juce::Point<float> end, const juce::String& paramId);
    float getDecaySustainMaxX() const;
    void addCurve(juce::Path& path, juce::Point<float> start, juce::Point<float> end, float value) const;

    juce::AudioProcessorValueTreeState& apvts;

    Thumb attack, decaySustain, release;
    Thumb attackCurve, decayCurve, releaseCurve;

    bool isSyncingFromParams = false;

    float w = 0.0f, h = 0.0f;
    int thumbSize = 15;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphAdsr)
};