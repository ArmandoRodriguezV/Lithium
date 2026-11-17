#include "Oscillator.h"
#include "../Palette/Palette.h"


Oscillator::Oscillator(
    juce::AudioProcessorValueTreeState& apvts,
    const juce::String& powerId,
    const juce::String& gainId, const juce::String& panId,
    const juce::String& transposeId, const juce::String& tuneId,
    const juce::String& waveFrameId, const juce::String& waveSelectorId)
    : viewer(apvts, waveFrameId, waveSelectorId),
    gain(apvts, gainId), pan(apvts, panId),
    transpose(apvts, transposeId), tune(apvts, tuneId)
{
    addAndMakeVisible(viewer);
    powerAttachment = std::make_unique<ButtonAttachment>(apvts, powerId, power);
    addAndMakeVisible(power);

    addAndMakeVisible(gain);
    gain.setSliderName("Gain");

    addAndMakeVisible(pan);
    pan.setSliderName("Pan");

    addAndMakeVisible(transpose);
    transpose.setSliderName("Transpose");

    addAndMakeVisible(tune);
    tune.setSliderName("Tune");
}

Oscillator::~Oscillator()
{
    gain.setLookAndFeel(nullptr);
    pan.setLookAndFeel(nullptr);
    transpose.setLookAndFeel(nullptr);
    tune.setLookAndFeel(nullptr);
    power.setLookAndFeel(nullptr);
}

void Oscillator::paint(juce::Graphics& g)
{
    g.fillAll(Palette::background);

    g.setColour(Palette::borderLight);
    g.drawRect(0, 0, getWidth(), getHeight());
}

void Oscillator::paintOverChildren(juce::Graphics &g)
{
    g.setColour(Palette::background.withAlpha(0.5f));
    if (!power.getToggleState())
    {
        g.fillRect(0, 0, getWidth(), getHeight());
    }
}

void Oscillator::resized()
{
    const auto w = getWidth();
    juce::Grid grid;

    power.setLookAndFeel(&s1);

    gain.setLookAndFeel(&s1);
    pan.setLookAndFeel(&s2);
    transpose.setLookAndFeel(&s3);
    tune.setLookAndFeel(&s3);

    grid.templateColumns = {
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1))
    };

    grid.templateRows = {
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1))
    };

    addItems(grid);
    grid.performLayout(getLocalBounds().withTrimmedLeft(10).withTrimmedRight(30).withTrimmedTop(10).withTrimmedBottom(10));
    power.setBounds(w - 30.0f, 5.0f, 25.0f, 25.0f);
}

void Oscillator::addItems(juce::Grid& grid)
{
    grid.items = {
        juce::GridItem(viewer).withArea(1, 1, 4, 5),
        juce::GridItem(gain).withArea(2, 5),
        juce::GridItem(pan).withArea(2, 6),
        juce::GridItem(transpose).withArea(1, 5),
        juce::GridItem(tune).withArea(1, 6),
    };
}

void Oscillator::setWaveFrame(const float* wF)
{
    viewer.setWaveFrame(wF);
}