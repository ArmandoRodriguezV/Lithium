#pragma once
#include <JuceHeader.h>

#include "../../../JuceLibraryCode/BinaryData.h"

enum class Category {
    Analogic,
    Digital,
    Noises,
    Monster
};

struct WaveTable
{
    int id;
    Category category;
    const void* data;
    size_t size;
    const char* name;
};

namespace WavetablesLibrary
{
    inline const WaveTable all[] =
    {
        { 0, Category::Analogic, BinaryData::basic_shapes_wav, BinaryData::basic_shapes_wavSize, "Basic shapes" },
        { 1, Category::Analogic, BinaryData::soft_transition_wav, BinaryData::soft_transition_wavSize, "Soft Transition" },
        { 2, Category::Analogic, BinaryData::analog_pulse_wav, BinaryData::analog_pulse_wavSize, "Analog pulse" },
        { 3, Category::Analogic, BinaryData::this_not_a_sine_wav, BinaryData::this_not_a_sine_wavSize, "This not a sine" },
        { 4, Category::Analogic, BinaryData::analog_fold_wav, BinaryData::analog_fold_wavSize, "Analog fold" },
        { 5, Category::Digital, BinaryData::strange_wav, BinaryData::strange_wavSize, "Strange" },
        { 6, Category::Digital, BinaryData::organic_chaos_wav, BinaryData::organic_chaos_wavSize, "Organic chaos" },
        { 7, Category::Digital, BinaryData::unreal_wav, BinaryData::unreal_wavSize, "Unreal" },
        { 8, Category::Digital, BinaryData::interrupt_wav, BinaryData::interrupt_wavSize, "Interrupt" },
        { 9, Category::Monster, BinaryData::monster_1_wav, BinaryData::monster_1_wavSize, "Monster 1" },
        { 10, Category::Monster, BinaryData::monster_2_wav, BinaryData::monster_2_wavSize, "Monster 2" },
        { 11, Category::Monster, BinaryData::monster_3_wav, BinaryData::monster_3_wavSize, "Monster 3" },
        { 12, Category::Monster, BinaryData::activity_cluster_wav, BinaryData::activity_cluster_wavSize, "Activity cluster" },
        { 13, Category::Digital, BinaryData::artificial_wav, BinaryData::artificial_wavSize, "Artificial" },
    };
    inline constexpr int allCount = sizeof(all) / sizeof(WaveTable);

    inline const WaveTable* find(int id)
    {
        for (int i = 0; i < allCount; ++i)
        {
            if (all[i].id == id)
                return &all[i];
        }

        return nullptr;
    }
}