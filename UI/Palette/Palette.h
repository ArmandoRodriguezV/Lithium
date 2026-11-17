#pragma once
#include <JuceHeader.h>

struct Palette
{
    // Main
    static inline const juce::Colour lithium = juce::Colour(0xFFB52A3C);
    static inline const juce::Colour lithiumLight = juce::Colour(0xFFE05A6B);
    static inline const juce::Colour lithiumDark = juce::Colour(0xFF7A1D2B);

    // Accent
    static inline const juce::Colour crimson = juce::Colour(0xFFD83A55);
    static inline const juce::Colour crimsonLight = juce::Colour(0xFFF06A7D);
    static inline const juce::Colour crimsonDark = juce::Colour(0xFF9E263C);

    // Backgrounds
    static inline const juce::Colour background = juce::Colour(0xFF111112);
    static inline const juce::Colour surface = juce::Colour(0xFF19191A);
    static inline const juce::Colour surfaceLight = juce::Colour(0xFF222223);
    static inline const juce::Colour surfaceDark = juce::Colour(0xFF0B0B0C);

    // Borders / separators
    static inline const juce::Colour border = juce::Colour(0xFF303032);
    static inline const juce::Colour borderLight = juce::Colour(0xFF454547);
    static inline const juce::Colour borderDark = juce::Colour(0xFF212123);

    // Text
    static inline const juce::Colour text = juce::Colour(0xFFF2F2F2);
    static inline const juce::Colour textSecondary = juce::Colour(0xFFB8B8BA);
    static inline const juce::Colour textDisabled = juce::Colour(0xFF6D6D70);

    // Controls
    static inline const juce::Colour control = juce::Colour(0xFF272729);
    static inline const juce::Colour controlHover = juce::Colour(0xFF333335);
    static inline const juce::Colour controlActive = juce::Colour(0xFFB52A3C);
    static inline const juce::Colour controlDisabled = juce::Colour(0xFF1D1D1F);

    // Graphs / visualization
    static inline const juce::Colour graph = juce::Colour(0xFFB52A3C);
    static inline const juce::Colour graphSecondary = juce::Colour(0xFFD83A55);
    static inline const juce::Colour graphBackground = juce::Colour(0xFF151516);
    static inline const juce::Colour graphGrid = juce::Colour(0xFF29292B);

    // Status
    static inline const juce::Colour success = juce::Colour(0xFF86C98A);
    static inline const juce::Colour warning = juce::Colour(0xFFE0B85C);
    static inline const juce::Colour error = crimson;
};