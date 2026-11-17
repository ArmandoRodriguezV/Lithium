#include "SVoice.h"
#include "SSound.h"

bool SVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SSound*>(sound) != nullptr;
}

void SVoice::pitchWheelMoved(int newPitchWheelValue)
{
    juce::ignoreUnused(newPitchWheelValue);
}

void SVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    voiceBuffer.clear();

    osc1.process(voiceBuffer);
    osc2.process(voiceBuffer);
    osc3.process(voiceBuffer);

    adsr.process(voiceBuffer, 0, numSamples);

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        outputBuffer.addFrom(channel, startSample, voiceBuffer, channel, 0, numSamples);

    if (adsr.isOff())
    {
        osc1.reset();
        osc2.reset();
        osc3.reset();
        clearCurrentNote();
    }
}

void SVoice::stopNote(float velocity, bool allowTailOff)
{
    juce::ignoreUnused(velocity);

    if (allowTailOff)
    {
        adsr.noteOff();
    } else {
        osc1.reset();
        osc2.reset();
        osc3.reset();
    }
}

void SVoice::setGain(float osc1G, float osc2G, float osc3G)
{
    osc1.setGain(osc1G);
    osc2.setGain(osc2G);
    osc3.setGain(osc3G);
}


void SVoice::setPan(float osc1P, float osc2P, float osc3P)
{
    osc1.setPan(osc1P);
    osc2.setPan(osc2P);
    osc3.setPan(osc3P);
}

void SVoice::setTranspose(int val1, int val2, int val3)
{
    osc1.setTranspose(val1);
    osc2.setTranspose(val2);
    osc3.setTranspose(val3);
}

void SVoice::setTune(int val1, int val2, int val3)
{
    osc1.setTune(val1);
    osc2.setTune(val2);
    osc3.setTune(val3);
}

void SVoice::setWaveframes1(const float *frame)
{
    osc1.setWaveframe(frame);
}

void SVoice::setWaveframes2(const float *frame)
{
    osc2.setWaveframe(frame);
}

void SVoice::setWaveframes3(const float *frame)
{
    osc3.setWaveframe(frame);
}

void SVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    juce::ignoreUnused(sound);
    juce::ignoreUnused(currentPitchWheelPosition);

    const float frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

    adsr.noteOn();

    osc1.setFrequency(frequency);
    osc1.noteOn(velocity);

    osc2.setFrequency(frequency);
    osc2.noteOn(velocity);

    osc3.setFrequency(frequency);
    osc3.noteOn(velocity);
}

void SVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
    juce::ignoreUnused(controllerNumber);
    juce::ignoreUnused(newControllerValue);
}

void SVoice::setAdsrValues(float a, float cA, float d, float cD, float s, float r, float cR)
{
    adsr.setAttack(a, cA);
    adsr.setDecay(d, cD);
    adsr.setSustain(s);
    adsr.setRelease(r, cR);
}

void SVoice::prepare(double sampleRate, int samplesPerBlock)
{
    voiceBuffer.setSize(2, samplesPerBlock);
    voiceBuffer.clear();

    adsr.prepare(sampleRate);
    osc1.prepare(sampleRate, samplesPerBlock);
    osc2.prepare(sampleRate, samplesPerBlock);
    osc3.prepare(sampleRate, samplesPerBlock);
}

void SVoice::powerOscillators(bool osc1S, bool osc2S, bool osc3S)
{
    osc1.setActive(osc1S);
    osc2.setActive(osc2S);
    osc3.setActive(osc3S);
}