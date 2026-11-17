#include "Osc.h"
#include "../Resources/Reader/WavetableReader.h"

void Osc::prepare(double newSampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);

    sampleRate = newSampleRate;

    setFrequency(frequency);
    reset();
}

void Osc::process(juce::AudioBuffer<float>& buffer)
{
    if (!active || waveframe == nullptr)
        return;

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const int index = static_cast<int>(phase * WavetableReader::tableSize) % WavetableReader::tableSize;

        float value = waveframe[index] * amplitude * gain;
        if (numChannels > 0)
            buffer.addSample(0, sample, value * leftGain);

        if (numChannels > 1)
            buffer.addSample(1, sample, value * rightGain);

        phase += phaseIncrement;

        if (phase >= 1.0f)
            phase -= 1.0f;
    }
}

void Osc::reset()
{
    phase = 0.0f;
    amplitude = 0.0f;
    active = false;
}

void Osc::setFrequency(float newFrequency)
{
    frequency = juce::jmax(0.0f, newFrequency);

    if (sampleRate > 0.0)
    {
        const float transposedFrequency = frequency * std::pow(2.0f, transpose / 12.0f) * std::pow(2.0, tune / 1200.0f);
        phaseIncrement = transposedFrequency / static_cast<float>(sampleRate);
    }
}

void Osc::setTranspose(int value)
{
    transpose = value;
    setFrequency(frequency);
}

void Osc::setTune(int val)
{
    tune = val;
    setFrequency(frequency);
}

void Osc::noteOn(float velocity)
{
    amplitude = velocity;
    active = true;
    phase = juce::jlimit(0.0f, 1.0f, phase + getRandomRange(0.0f, 1.0f));
}

void Osc::setGain(float g)
{
    gain = g;
}

void Osc::setPan(float newPan)
{
    pan = juce::jlimit(-1.0f, 1.0f, newPan);

    const float angle = (pan + 1.0f) * juce::MathConstants<float>::pi * 0.25f;

    leftGain = std::cos(angle);
    rightGain = std::sin(angle);
}

void Osc::setActive(bool shouldBeActive)
{
    active = shouldBeActive;

    if (!active)
    {
        amplitude = 0.0f;
        gain = 0.0f;
    }
}

bool Osc::getState()
{
    return active;
}

void Osc::setWaveframe(const float *waveform)
{
    waveframe = waveform;
}

float Osc::getRandomRange(const float minValue, const float maxValue)
{
    if (minValue >= maxValue)
        return maxValue;

    const float randomVal = rand.nextFloat();
    const float result = juce::jmap(randomVal, 0.0f, 1.0f, minValue, maxValue);
    return result;
}
