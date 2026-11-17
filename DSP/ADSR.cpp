#include "ADSR.h"

void ADSR::prepare(double sR)
{
    sampleRate = sR;
}

void ADSR::setAttack(const float a, const float c)
{
    attack = a;
    attackCurve = c;
    attackSamples = static_cast<int>(attack * sampleRate);
}

void ADSR::setDecay(const float d, const float c)
{
    decay = d;
    decayCurve = c;
    decaySamples = static_cast<int>(decay * sampleRate);
}

void ADSR::setSustain(const float s)
{
    sustainLevel = juce::jlimit(0.0f, 1.0f, s);
}

void ADSR::setRelease(const float r, const float c)
{
    release = r;
    releaseCurve = c;
    releaseSamples = static_cast<int>(release * sampleRate);
}

void ADSR::noteOn()
{
    currentSample = 0;
    stage = Attack;
}

void ADSR::noteOff()
{
    releaseStartLevel = currentLevel;
    currentSample = 0;
    stage = Release;
}
void ADSR::process(juce::AudioBuffer<float>& buffer, int startSample, int samplesPerBlock)
{
    for (int sp = 0; sp < samplesPerBlock; ++sp)
    {
        float envelope = 0.0f;

        switch (stage)
        {
            case Attack:
            {
                float progress = static_cast<float>(currentSample) / static_cast<float>(attackSamples);
                progress = juce::jlimit(0.0f, 1.0f, progress);
                envelope = std::pow(progress, 1.0f / attackCurve);

                currentLevel = envelope;

                currentSample++;
                if (currentSample >= attackSamples)
                {
                    stage = Decay;
                    currentSample = 0;
                }
                break;
            }

            case Decay:
            {
                float progress = static_cast<float>(currentSample) / static_cast<float>(decaySamples);
                progress = juce::jlimit(0.0f, 1.0f, progress);

                float curvedProgress = std::pow(progress, decayCurve);
                envelope = juce::jmap(curvedProgress, 1.0f, sustainLevel);

                currentLevel = envelope;

                currentSample++;
                if (currentSample >= decaySamples)
                {
                    stage = Sustain;
                    currentSample = 0;
                }
                break;
            }

            case Sustain:
            {
                envelope = sustainLevel;
                currentLevel = envelope;
                break;
            }

            case Release:
            {
                if (releaseSamples <= 0)
                {
                    envelope = 0.0f;
                    currentLevel = 0.0f;
                    stage = Off;
                    break;
                }

                float progress = static_cast<float>(currentSample) / static_cast<float>(releaseSamples);
                progress = juce::jlimit(0.0f, 1.0f, progress);

                float curvedProgress = std::pow(progress, releaseCurve);

                envelope = releaseStartLevel * (1.0f - curvedProgress);
                currentLevel = envelope;

                currentSample++;

                if (currentSample >= releaseSamples)
                {
                    envelope = 0.0f;
                    currentLevel = 0.0f;
                    stage = Off;
                    currentSample = 0;
                }

                break;
            }

            case Off:
            {
                envelope = 0.0f;
                break;
            }
        }

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* output = buffer.getWritePointer(channel);
            output[startSample + sp] *= envelope;
        }
    }
}

bool ADSR::isOff()
{
    return stage == Off;
}



