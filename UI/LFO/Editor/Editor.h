#pragma once
#include <JuceHeader.h>
#include "../../Components/Thumb/Thumb.h"

class Editor: public juce::Component
{
public:
    Editor();
    ~Editor();

    void paint(juce::Graphics &g) override;
    void resized() override;

private:
    std::vector<Thumb> thumbs;
};