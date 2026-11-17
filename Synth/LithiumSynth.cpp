#include "LithiumSynth.h"
#include "../Resources/Library/WavetablesLibrary.h"

LithiumSynth::LithiumSynth()
{
    reader1.load(BinaryData::soft_transition_wav, BinaryData::soft_transition_wavSize);
    reader2.load(BinaryData::soft_transition_wav, BinaryData::soft_transition_wavSize);
    reader3.load(BinaryData::soft_transition_wav, BinaryData::soft_transition_wavSize);

    constexpr int numVoices = 16;

    for (int i = 0; i < numVoices; ++i)
    {
        auto voice = new SVoice();
        voices.push_back(voice);
        synth.addVoice(voice);
    }

    synth.addSound(new SSound());
}

void LithiumSynth::prepare(const juce::dsp::ProcessSpec &spec) {
    for (auto voice : voices)
    {
        voice->prepare(spec.sampleRate, spec.maximumBlockSize);
    }
}

void LithiumSynth::process(juce::AudioBuffer<float> &buffer, const juce::MidiBuffer &midiMessages)
{
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    auto* wf1 = reader1.getTable(readerWf1).data();
    auto* wf2 = reader2.getTable(readerWf2).data();
    auto* wf3 = reader3.getTable(readerWf3).data();

    WavetableReader::setWaveFrame1(wf1);
    WavetableReader::setWaveFrame2(wf2);
    WavetableReader::setWaveFrame3(wf3);

    for (auto voice : voices)
    {
        voice->setWaveframes1(wf1);
        voice->setWaveframes2(wf2);
        voice->setWaveframes3(wf3);
    }
}

void LithiumSynth::powerOscillators(bool osc1, bool osc2, bool osc3) const {
    for (auto voice : voices)
    {
        voice->powerOscillators(osc1, osc2, osc3);
    }
}

void LithiumSynth::setAdsrValues(float a, float cA, float d, float cD, float s, float r, float cR) const {
    s = juce::Decibels::decibelsToGain(s, -60.0f);

    for (auto voice : voices)
        voice->setAdsrValues(a, cA, d, cD, s, r, cR);
}

void LithiumSynth::setOscGain(float osc1G, float osc2G, float osc3G)
{
    osc1G = juce::Decibels::decibelsToGain(osc1G, -60.0f);
    osc2G = juce::Decibels::decibelsToGain(osc2G, -60.0f);
    osc3G = juce::Decibels::decibelsToGain(osc3G, -60.0f);

    for (auto voice : voices)
    {
        voice->setGain(osc1G, osc2G, osc3G);
    }
}

void LithiumSynth::setOscPan(float osc1P, float osc2P, float osc3P)
{
    for (auto voice : voices)
    {
        voice->setPan(osc1P, osc2P, osc3P);
    }
}

void LithiumSynth::setTranspose(int val1, int val2, int val3)
{
    for (auto voice : voices)
    {
        voice->setTranspose(val1, val2, val3);
    }
}

void LithiumSynth::setTune(int val1, int val2, int val3)
{
    for (auto voice : voices)
    {
        voice->setTune(val1, val2, val3);
    }
}

void LithiumSynth::setWaveFrameOsc1(float val)
{
    readerWf1 = static_cast<int>(std::round(val * (reader1.getTotalTables() - 1)));
}

void LithiumSynth::setWaveFrameOsc2(float val)
{
    readerWf2 = static_cast<int>(std::round(val * (reader2.getTotalTables() - 1)));
}

void LithiumSynth::setWaveFrameOsc3(float val)
{
    readerWf3 = static_cast<int>(std::round(val * (reader3.getTotalTables() - 1)));
}

void LithiumSynth::loadWaveformIntoOscillator1(int osc)
{
    const auto* tableOsc = WavetablesLibrary::find(osc);

    if (tableOsc != nullptr)
        reader1.load(tableOsc->data, tableOsc->size);
}

void LithiumSynth::loadWaveformIntoOscillator2(int osc)
{
    const auto* tableOsc = WavetablesLibrary::find(osc);

    if (tableOsc != nullptr)
        reader2.load(tableOsc->data, tableOsc->size);
}

void LithiumSynth::loadWaveformIntoOscillator3(int osc)
{
    const auto* tableOsc = WavetablesLibrary::find(osc);

    if (tableOsc != nullptr)
        reader3.load(tableOsc->data, tableOsc->size);
}

