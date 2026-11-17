#pragma once
#include <JuceHeader.h>
#include "Synth/LithiumSynth.h"

//==============================================================================
/**
*/
class LithiumAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    LithiumAudioProcessor();
    ~LithiumAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

private:
    LithiumSynth synth;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameter();

    void loadOscillatorsWaveTables();
    int lastOsc1Wave = -1;
    int lastOsc2Wave = -1;
    int lastOsc3Wave = -1;
    void oscillatorsConfig();

    void adsrProcess();

    void createOscParameters(std::vector<std::unique_ptr<juce::RangedAudioParameter>>& params);
    void createADSRParameters(std::vector<std::unique_ptr<juce::RangedAudioParameter>>& params);
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LithiumAudioProcessor)
};
