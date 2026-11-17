#pragma once

struct ApvtsParams
{
    // Oscillators
    static constexpr const char* Osc1Gain = "OSC1_GAIN_ID";
    static constexpr const char* Osc2Gain = "OSC2_GAIN_ID";
    static constexpr const char* Osc3Gain = "OSC3_GAIN_ID";

    static constexpr const char* Osc1Pan = "OSC1_PAN_ID";
    static constexpr const char* Osc2Pan = "OSC2_PAN_ID";
    static constexpr const char* Osc3Pan = "OSC3_PAN_ID";

    static constexpr const char* Osc1Transpose = "OSC1_TRANSPOSE_ID";
    static constexpr const char* Osc2Transpose = "OSC2_TRANSPOSE_ID";
    static constexpr const char* Osc3Transpose = "OSC3_TRANSPOSE_ID";

    static constexpr const char* Osc1Tune = "OSC1_TUNE_ID";
    static constexpr const char* Osc2Tune = "OSC2_TUNE_ID";
    static constexpr const char* Osc3Tune = "OSC3_TUNE_ID";

    static constexpr const char* Osc1Waveframe = "OSC1_WAVE_FRAME_ID";
    static constexpr const char* Osc2Waveframe = "OSC2_WAVE_FRAME_ID";
    static constexpr const char* Osc3Waveframe = "OSC3_WAVE_FRAME_ID";

    static constexpr const char* Osc1WaveSelector = "OSC1_WAVE_SELECTOR_ID";
    static constexpr const char* Osc2WaveSelector = "OSC2_WAVE_SELECTOR_ID";
    static constexpr const char* Osc3WaveSelector = "OSC3_WAVE_SELECTOR_ID";

    static constexpr const char* Osc1OnOrOffState = "OSC1_POWER_ID";
    static constexpr const char* Osc2OnOrOffState = "OSC2_POWER_ID";
    static constexpr const char* Osc3OnOrOffState = "OSC3_POWER_ID";

    // ADSR
    static constexpr const char* AttackId = "ATTACK_ID";
    static constexpr const char* AttackCurveId = "ATTACK_C_ID";

    static constexpr const char* DecayId = "DECAY_ID";
    static constexpr const char* DecayCurveId = "DECAY_C_ID";

    static constexpr const char* SustainId = "SUSTAIN_ID";

    static constexpr const char* ReleaseId = "RELEASE_ID";
    static constexpr const char* ReleaseCurveId = "RELEASE_C_ID";
};