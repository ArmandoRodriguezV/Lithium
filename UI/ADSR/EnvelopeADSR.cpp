#include "EnvelopeADSR.h"
#include "../../Context/ApvtsParams.h"

EnvelopeADSR::EnvelopeADSR(juce::AudioProcessorValueTreeState& apvts):
    graphAdsr(apvts),
    attack(apvts, ApvtsParams::AttackId),
    decay(apvts, ApvtsParams::DecayId),
    sustain(apvts, ApvtsParams::SustainId),
    release(apvts, ApvtsParams::ReleaseId)
{
    addAndMakeVisible(attack);
    addAndMakeVisible(decay);
    addAndMakeVisible(sustain);
    addAndMakeVisible(release);

    addAndMakeVisible(graphAdsr);
}

EnvelopeADSR::~EnvelopeADSR()
{
    attack.setLookAndFeel(nullptr);
    decay.setLookAndFeel(nullptr);
    sustain.setLookAndFeel(nullptr);
    release.setLookAndFeel(nullptr);
}

void EnvelopeADSR::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colour(20, 20, 20));
    g.setColour(juce::Colour(120, 120, 120));

    g.drawRect(0, 0, getWidth(), getHeight());
}

void EnvelopeADSR::resized()
{
    attack.setSliderName("Attack");
    attack.setLookAndFeel(&style1);

    decay.setLookAndFeel(&style1);
    decay.setSliderName("Decay");

    sustain.setLookAndFeel(&style1);
    sustain.setSliderName("Sustain");

    release.setLookAndFeel(&style1);
    release.setSliderName("Release");

    juce::Grid grid;
    grid.templateColumns = {
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
    };

    grid.templateRows = {
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1))
    };

    positionedToGrid(grid);
    grid.performLayout(getLocalBounds().reduced(10));
}

void EnvelopeADSR::positionedToGrid(juce::Grid& g)
{
    g.items = {
        juce::GridItem(attack).withArea(1, 1),
        juce::GridItem(decay).withArea(1, 2),
        juce::GridItem(sustain).withArea(2, 1),
        juce::GridItem(release).withArea(2, 2),
        juce::GridItem(graphAdsr).withArea(1, 3, 3, 7)
    };
}


