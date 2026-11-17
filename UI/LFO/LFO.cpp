#include "LFO.h"
#include "../Palette/Palette.h"

LFO::LFO(juce::AudioProcessorValueTreeState& apvts)
{

}

LFO::~LFO()
{
}

void LFO::paint(juce::Graphics& g)
{
    g.fillAll(Palette::background);

}

void LFO::resized()
{
    juce::Grid grid;
}
