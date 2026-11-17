#include "WavetableReader.h"

const float* WavetableReader::waveFrame1 = nullptr;
const float* WavetableReader::waveFrame2 = nullptr;
const float* WavetableReader::waveFrame3 = nullptr;

const std::vector<std::vector<float>>& WavetableReader::getTables() const
{
    return tables;
}

int WavetableReader::getTotalTables() const
{
    return static_cast<int>(tables.size());
}

const std::vector<float>& WavetableReader::getTable(int index) const
{
    jassert(index >= 0 && index < static_cast<int>(tables.size()));
    return tables[index];
}

bool WavetableReader::load(const void* data, size_t size)
{
    tables.clear();

    if (data == nullptr || size == 0)
        return false;

    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    auto inputStream = std::make_unique<juce::MemoryInputStream>(data, size, false);

    std::unique_ptr<juce::AudioFormatReader> reader(
        formatManager.createReaderFor(std::move(inputStream))
    );

    if (reader == nullptr)
        return false;

    if (reader->numChannels <= 0 || reader->lengthInSamples < tableSize)
        return false;

    juce::AudioBuffer<float> sourceBuffer(
        static_cast<int>(reader->numChannels),
        static_cast<int>(reader->lengthInSamples)
    );

    if (!reader->read(
        &sourceBuffer,
        0,
        static_cast<int>(reader->lengthInSamples),
        0,
        true,
        true))
        return false;

    const int numSamples = sourceBuffer.getNumSamples();
    const int numTables = numSamples / tableSize;

    tables.reserve(numTables);

    for (int table = 0; table < numTables; ++table)
    {
        const int sourceStart = table * tableSize;

        std::vector<float> waveframe(tableSize);

        for (int sample = 0; sample < tableSize; ++sample)
        {
            float value = 0.0f;

            for (int channel = 0; channel < sourceBuffer.getNumChannels(); ++channel)
                value += sourceBuffer.getSample(channel, sourceStart + sample);

            value /= static_cast<float>(sourceBuffer.getNumChannels());

            waveframe[sample] = value;
        }

        tables.push_back(std::move(waveframe));
    }

    return !tables.empty();
}