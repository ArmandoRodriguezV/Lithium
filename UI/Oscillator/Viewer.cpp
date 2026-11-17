#include "Viewer.h"
#include "../../Resources/Reader/WavetableReader.h"
#include "../Palette/Palette.h"

Viewer::Viewer(
    juce::AudioProcessorValueTreeState& apvtsRef,
    juce::String waveFrameId,
    juce::String waveSelectorIdRef)
    : apvts(apvtsRef),
      waveSelectorId(waveSelectorIdRef)
{
    addAndMakeVisible(waveFrameSlider);
    waveFrameAttachment = std::make_unique<SliderAttachment>(
        apvts,
        waveFrameId,
        waveFrameSlider
    );

    addAndMakeVisible(wavetableMenuBtn);
    updateWaveTableButton();

    wavetableMenuBtn.onClick = [this]()
    {
        createWaveTableMenu();
    };
}

Viewer::~Viewer()
{
}

void Viewer::paint(juce::Graphics& g)
{
    g.setColour(Palette::border);
    g.fillRect(0, 0, getWidth(), getHeight());

    const auto bounds = getLocalBounds().toFloat();
    const float width = bounds.getWidth() - sliderSize;
    const float height = bounds.getHeight() - selectorHeight;
    const float centreY = selectorHeight + height * 0.5f;

    g.setColour(Palette::border);
    g.drawRect(0.0f, selectorHeight, sliderSize + width, height);

    if (waveFrame == nullptr)
        return;

    juce::Path waveform;

    const float firstX = sliderSize;
    const float lastX = sliderSize + width;

    waveform.startNewSubPath(firstX, centreY);

    for (int i = 0; i < WavetableReader::tableSize; ++i)
    {
        const float x = static_cast<float>(i) / (WavetableReader::tableSize - 1) * width + sliderSize;
        const float y = centreY - waveFrame[i] * (height * 0.5f);

        waveform.lineTo(x, y);
    }

    waveform.lineTo(lastX, centreY);
    waveform.closeSubPath();

    g.setColour(Palette::crimsonDark);
    g.fillPath(waveform);

    g.setColour(Palette::crimsonDark);
    g.drawHorizontalLine(static_cast<int>(centreY), sliderSize, lastX);

    g.setColour(Palette::crimsonLight);
    g.strokePath(waveform, juce::PathStrokeType(1.0f));

}

void Viewer::resized()
{
    waveFrameSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    waveFrameSlider.setTextBoxStyle(juce::Slider::NoTextBox, 0.0f, 0.0f, false);
    waveFrameSlider.setBounds(0, 0, static_cast<int>(sliderSize), getHeight());

    wavetableMenuBtn.setBounds(static_cast<int>(sliderSize), 0, getWidth() - static_cast<int>(sliderSize), static_cast<int>(selectorHeight));
}

void Viewer::setWaveFrame(const float* wF)
{
    waveFrame = wF;
    repaint();
}

void Viewer::createWaveTableMenu()
{
    juce::PopupMenu menu;
    juce::PopupMenu analogicMenu;
    juce::PopupMenu digitalMenu;
    juce::PopupMenu noiseMenu;
    juce::PopupMenu monsterMenu;

    for (int i = 0; i < WavetablesLibrary::allCount; ++i)
    {
        const auto& wt = WavetablesLibrary::all[i];
        const int menuId = wt.id + 1;

        if (wt.category == Category::Analogic)
            analogicMenu.addItem(menuId, wt.name);

        if (wt.category == Category::Digital)
            digitalMenu.addItem(menuId, wt.name);

        if (wt.category == Category::Noises)
            noiseMenu.addItem(menuId, wt.name);

        if (wt.category == Category::Monster)
            monsterMenu.addItem(menuId, wt.name);
    }

    menu.addSubMenu("Analogic", analogicMenu);
    menu.addSubMenu("Digital", digitalMenu);
    menu.addSubMenu("Noises", noiseMenu);
    menu.addSubMenu("Monster", monsterMenu);

    menu.showMenuAsync(
        juce::PopupMenu::Options().withTargetComponent(&wavetableMenuBtn),
        [this](int result)
        {
            if (result <= 0)
                return;

            const int wavetableId = result - 1;

            if (auto* parameter = apvts.getParameter(waveSelectorId))
            {
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(
                        static_cast<float>(wavetableId)
                    )
                );

                updateWaveTableButton();
            }
        }
    );
}

void Viewer::updateWaveTableButton()
{
    if (auto* parameter = apvts.getParameter(waveSelectorId))
    {
        const int index = static_cast<int>(
            parameter->convertFrom0to1(parameter->getValue())
        );

        if (index >= 0 && index < WavetablesLibrary::allCount)
            wavetableMenuBtn.setButtonText(WavetablesLibrary::all[index].name);
    }
}