#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Resources/Reader/WavetableReader.h"
#include "Context/ApvtsParams.h"

LithiumAudioProcessorEditor::LithiumAudioProcessorEditor (LithiumAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    osc1(audioProcessor.apvts,
        ApvtsParams::Osc1OnOrOffState,
        ApvtsParams::Osc1Gain, ApvtsParams::Osc1Pan,
        ApvtsParams::Osc1Transpose, ApvtsParams::Osc1Tune,
        ApvtsParams::Osc1Waveframe, ApvtsParams::Osc1WaveSelector),
    osc2(audioProcessor.apvts,
        ApvtsParams::Osc2OnOrOffState,
        ApvtsParams::Osc2Gain, ApvtsParams::Osc2Pan,
        ApvtsParams::Osc2Transpose, ApvtsParams::Osc2Tune,
        ApvtsParams::Osc2Waveframe, ApvtsParams::Osc2WaveSelector),
    osc3(audioProcessor.apvts,
        ApvtsParams::Osc3OnOrOffState,
        ApvtsParams::Osc3Gain, ApvtsParams::Osc3Pan,
        ApvtsParams::Osc3Transpose, ApvtsParams::Osc3Tune,
        ApvtsParams::Osc3Waveframe, ApvtsParams::Osc3WaveSelector),
    envelopeAdsr(audioProcessor.apvts),
    lfo(audioProcessor.apvts)
{
    startTimerHz(40);
    setSize (1300, 800);

    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);
    addAndMakeVisible(osc3);
    addAndMakeVisible(envelopeAdsr);
    addAndMakeVisible(lfo);
}

LithiumAudioProcessorEditor::~LithiumAudioProcessorEditor()
{
}

void LithiumAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void LithiumAudioProcessorEditor::resized()
{
    auto w = getWidth();
    auto h = getHeight();

    auto oscSizeW = w * 0.5;
    auto oscSizeH = h / 3.0;

    osc1.setBounds(0, 0, oscSizeW, oscSizeH);
    osc2.setBounds(0, oscSizeH, oscSizeW, oscSizeH);
    osc3.setBounds(0, oscSizeH * 2, oscSizeW, oscSizeH);

    lfo.setBounds(w * 0.5f, h * 0.5f, w * 0.5f, h * 0.5f);

    envelopeAdsr.setBounds(w * 0.5f, 0.0f, w * 0.5f, h * 0.25f);
}

void LithiumAudioProcessorEditor::timerCallback()
{
    osc1.setWaveFrame(WavetableReader::getWaveFrame1());
    osc1.repaint();

    osc2.setWaveFrame(WavetableReader::getWaveFrame2());
    osc2.repaint();

    osc3.setWaveFrame(WavetableReader::getWaveFrame3());
    osc3.repaint();
}
