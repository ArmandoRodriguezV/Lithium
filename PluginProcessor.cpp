#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Context/ApvtsParams.h"
#include "Resources/Library/WavetablesLibrary.h"

//==============================================================================
LithiumAudioProcessor::LithiumAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "PARAMETER", createParameter())
#endif
{
}

LithiumAudioProcessor::~LithiumAudioProcessor()
{
}

//==============================================================================
const juce::String LithiumAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LithiumAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LithiumAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LithiumAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LithiumAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LithiumAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LithiumAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LithiumAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LithiumAudioProcessor::getProgramName (int index)
{
    return {};
}

void LithiumAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LithiumAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec {};
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    synth.prepare(spec);
}

void LithiumAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LithiumAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LithiumAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    loadOscillatorsWaveTables();
    oscillatorsConfig();
    adsrProcess();

    synth.process(buffer, midiMessages);
}

void LithiumAudioProcessor::adsrProcess()
{
    synth.setAdsrValues(
        apvts.getRawParameterValue(ApvtsParams::AttackId)->load(),
        apvts.getRawParameterValue(ApvtsParams::AttackCurveId)->load(),
        apvts.getRawParameterValue(ApvtsParams::DecayId)->load(),
        apvts.getRawParameterValue(ApvtsParams::DecayCurveId)->load(),
        apvts.getRawParameterValue(ApvtsParams::SustainId)->load(),
        apvts.getRawParameterValue(ApvtsParams::ReleaseId)->load(),
        apvts.getRawParameterValue(ApvtsParams::ReleaseCurveId)->load()
    );
}

void LithiumAudioProcessor::loadOscillatorsWaveTables()
{
    const int osc1Wave = static_cast<int>(apvts.getRawParameterValue(ApvtsParams::Osc1WaveSelector)->load());
    const int osc2Wave = static_cast<int>(apvts.getRawParameterValue(ApvtsParams::Osc2WaveSelector)->load());
    const int osc3Wave = static_cast<int>(apvts.getRawParameterValue(ApvtsParams::Osc3WaveSelector)->load());

    if (osc1Wave != lastOsc1Wave || osc2Wave != lastOsc2Wave || osc3Wave != lastOsc3Wave)
    {
        synth.loadWaveformIntoOscillator1(osc1Wave);
        synth.loadWaveformIntoOscillator2(osc2Wave);
        synth.loadWaveformIntoOscillator3(osc3Wave);

        lastOsc1Wave = osc1Wave;
        lastOsc2Wave = osc2Wave;
        lastOsc3Wave = osc3Wave;
    }
}

void LithiumAudioProcessor::oscillatorsConfig()
{
    synth.setWaveFrameOsc1(*apvts.getRawParameterValue(ApvtsParams::Osc1Waveframe));
    synth.setWaveFrameOsc2(*apvts.getRawParameterValue(ApvtsParams::Osc2Waveframe));
    synth.setWaveFrameOsc3(*apvts.getRawParameterValue(ApvtsParams::Osc3Waveframe));

    synth.setOscGain(
        apvts.getRawParameterValue(ApvtsParams::Osc1Gain)->load(),
        apvts.getRawParameterValue(ApvtsParams::Osc2Gain)->load(),
        apvts.getRawParameterValue(ApvtsParams::Osc3Gain)->load()
    );

    synth.setOscPan(
        apvts.getRawParameterValue(ApvtsParams::Osc1Pan)->load(),
        apvts.getRawParameterValue(ApvtsParams::Osc2Pan)->load(),
        apvts.getRawParameterValue(ApvtsParams::Osc3Pan)->load()
    );

    synth.powerOscillators(
        apvts.getRawParameterValue(ApvtsParams::Osc1OnOrOffState)->load(),
        apvts.getRawParameterValue(ApvtsParams::Osc2OnOrOffState)->load(),
        apvts.getRawParameterValue(ApvtsParams::Osc3OnOrOffState)->load()
    );

    synth.setTranspose(
        apvts.getRawParameterValue(ApvtsParams::Osc1Transpose)->load(),
        apvts.getRawParameterValue(ApvtsParams::Osc2Transpose)->load(),
        apvts.getRawParameterValue(ApvtsParams::Osc3Transpose)->load()
    );

    synth.setTune(
        apvts.getRawParameterValue(ApvtsParams::Osc1Tune)->load(),
        apvts.getRawParameterValue(ApvtsParams::Osc2Tune)->load(),
        apvts.getRawParameterValue(ApvtsParams::Osc3Tune)->load()
    );
}

juce::AudioProcessorValueTreeState::ParameterLayout LithiumAudioProcessor::createParameter()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    createOscParameters(params);
    createADSRParameters(params);
    return { params.begin(), params.end() };
}

void LithiumAudioProcessor::createOscParameters(std::vector<std::unique_ptr<juce::RangedAudioParameter>>& params)
{

    // Osc Gain
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::Osc1Gain, "Osc 1 Gain", -60.0f, 12.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::Osc2Gain, "Osc 2 Gain", -60.0f, 12.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::Osc3Gain, "Osc 3 Gain", -60.0f, 12.0f, 0.0f));

    // Osc Pan
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::Osc1Pan, "Osc 1 Pan", -1.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::Osc2Pan, "Osc 2 Pan", -1.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::Osc3Pan, "Osc 3 Pan", -1.0f, 1.0f, 0.0f));

    // Osc wave frame
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::Osc1Waveframe, "Osc 1 wave frame", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::Osc2Waveframe, "Osc 2 wave frame", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::Osc3Waveframe, "Osc 3 wave frame", 0.0f, 1.0f, 0.0f));

    // Osc transpose
    params.push_back(std::make_unique<juce::AudioParameterInt>(ApvtsParams::Osc1Transpose, "Osc 1 transpose", -48, 48, 0));
    params.push_back(std::make_unique<juce::AudioParameterInt>(ApvtsParams::Osc2Transpose, "Osc 2 transpose", -48, 48, 0));
    params.push_back(std::make_unique<juce::AudioParameterInt>(ApvtsParams::Osc3Transpose, "Osc 3 transpose", -48, 48, 0));

    // Osc Tone
    params.push_back(std::make_unique<juce::AudioParameterInt>(ApvtsParams::Osc1Tune, "Osc 1 tune", -100, 100, 0));
    params.push_back(std::make_unique<juce::AudioParameterInt>(ApvtsParams::Osc2Tune, "Osc 2 tune", -100, 100, 0));
    params.push_back(std::make_unique<juce::AudioParameterInt>(ApvtsParams::Osc3Tune, "Osc 3 tune", -100, 100, 0));

    // Select Wave table
    juce::StringArray wavetableNames;
    for (int i = 0; i < WavetablesLibrary::allCount; ++i)
        wavetableNames.add(WavetablesLibrary::all[i].name);

    params.push_back(std::make_unique<juce::AudioParameterChoice>(ApvtsParams::Osc1WaveSelector, "Osc 1 Choice", wavetableNames, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(ApvtsParams::Osc2WaveSelector, "Osc 2 Choice", wavetableNames, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(ApvtsParams::Osc3WaveSelector, "Osc 3 Choice", wavetableNames, 0));

    // Active Osc
    params.push_back(std::make_unique<juce::AudioParameterBool>(ApvtsParams::Osc1OnOrOffState, "Osc 1", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>(ApvtsParams::Osc2OnOrOffState, "Osc 2", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>(ApvtsParams::Osc3OnOrOffState, "Osc 3", false));
}

void LithiumAudioProcessor::createADSRParameters(std::vector<std::unique_ptr<juce::RangedAudioParameter>>& params)
{
    constexpr auto minValue = 0.015625f;
    constexpr auto maxValue = 64.0f;

    juce::NormalisableRange<float> range (
        minValue,
        maxValue,
        [](const float start, const float end, const float normalised)
        {
            return start * std::pow (end / start, normalised);
        },
        [](const float start, const float end, const float value)
        {
            const auto clampedValue = juce::jlimit (start, end, value);
            return std::log (clampedValue / start) / std::log (end / start);
        },
        [](const float start, const float end, const float value)
        {
            return juce::jlimit (start, end, value);
        }
    );

    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::AttackId, "Attack", 0.0f, 5.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::AttackCurveId, "Attack curve", range, 1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::DecayId, "Decay", 0.0f, 5.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::DecayCurveId, "Decay curve", range, 1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::SustainId, "Sustain", -60.0f, 0.0f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::ReleaseId, "Release", 0.0f, 5.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(ApvtsParams::ReleaseCurveId, "Release curve", range, 1.0f));
}

//==============================================================================
bool LithiumAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LithiumAudioProcessor::createEditor()
{
    return new LithiumAudioProcessorEditor (*this);
}

//==============================================================================
void LithiumAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();

    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void LithiumAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

    if (xml != nullptr)
    {
        auto state = juce::ValueTree::fromXml(*xml);

        if (state.isValid())
            apvts.replaceState(state);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LithiumAudioProcessor();
}
