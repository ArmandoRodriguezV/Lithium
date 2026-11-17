#pragma once
#include <JuceHeader.h>

class SSound: public juce::SynthesiserSound
{
public:
    bool appliesToChannel(int midiChannel) override;
    bool appliesToNote(int midiNoteNumber) override;
};