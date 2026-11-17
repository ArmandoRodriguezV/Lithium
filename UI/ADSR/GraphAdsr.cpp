#include "GraphAdsr.h"
#include "../../Context/ApvtsParams.h"
#include "../../UI/Palette/Palette.h"

static float curveExponent(float value)
{
    constexpr float minExponent = 1.0f / 64.0f;
    constexpr float maxExponent = 64.0f;
    return minExponent * std::pow(maxExponent / minExponent, value);
}

static float curveValueFromT(float curveT)
{
    constexpr float minExponent = 1.0f / 64.0f;
    constexpr float maxExponent = 64.0f;

    const float clampedT = juce::jlimit(0.0001f, 0.9999f, curveT);
    const float exponent = juce::jlimit(minExponent, maxExponent, std::log(clampedT) / std::log(0.5f));
    return std::log(exponent / minExponent) / std::log(maxExponent / minExponent);
}

GraphAdsr::GraphAdsr(juce::AudioProcessorValueTreeState& apvts):
    apvts(apvts)
{
    startTimerHz(30);

    addAndMakeVisible(attack);
    addAndMakeVisible(decaySustain);
    addAndMakeVisible(release);
    addAndMakeVisible(attackCurve);
    addAndMakeVisible(decayCurve);
    addAndMakeVisible(releaseCurve);

    attack.onPositionChanged = [this, &apvts]()
    {
        if (isSyncingFromParams) return;
        const auto val = attack.getCenterPosition().getX();
        const auto realValue = juce::jmap(val, 0.0f, w / 3.0f, 0.0f, 1.0f);
        apvts.getParameter(ApvtsParams::AttackId)->setValueNotifyingHost(juce::jlimit(0.0f, 1.0f, realValue));
        repaint();
    };

    decaySustain.onPositionChanged = [this, &apvts]()
    {
        if (isSyncingFromParams) return;
        const auto pos = decaySustain.getCenterPosition();
        const auto minX = attack.getCenterPosition().getX();
        const auto maxX = getDecaySustainMaxX();
        const auto decayValue = (maxX > minX) ? juce::jmap(pos.getX(), minX, maxX, 0.0f, 1.0f) : 0.0f;
        const auto sustainValue = juce::jmap(pos.getY(), 0.0f, h, 1.0f, 0.0f);
        apvts.getParameter(ApvtsParams::DecayId)->setValueNotifyingHost(juce::jlimit(0.0f, 1.0f, decayValue));
        apvts.getParameter(ApvtsParams::SustainId)->setValueNotifyingHost(juce::jlimit(0.0f, 1.0f, sustainValue));
        repaint();
    };

    release.onPositionChanged = [this, &apvts]()
    {
        if (isSyncingFromParams) return;
        const auto x = release.getCenterPosition().getX();
        const auto minX = decaySustain.getCenterPosition().getX();
        const auto realValue = (w > minX) ? juce::jmap(x, minX, w, 0.0f, 1.0f) : 0.0f;
        apvts.getParameter(ApvtsParams::ReleaseId)->setValueNotifyingHost(juce::jlimit(0.0f, 1.0f, realValue));
        repaint();
    };

    attackCurve.setType(ThumbType::Curve);
    attackCurve.onPositionChanged = [this, &apvts]()
    {
        if (isSyncingFromParams) return;
        const auto start = juce::Point<float>(0.0f, h);
        const auto end = attack.getCenterPosition();
        const auto y = attackCurve.getCenterPosition().getY();
        const auto curveT = (end.getY() != start.getY())
            ? juce::jmap(y, start.getY(), end.getY(), 0.0f, 1.0f)
            : 0.5f;
        const auto val = curveValueFromT(curveT);
        apvts.getParameter(ApvtsParams::AttackCurveId)->setValueNotifyingHost(juce::jlimit(0.0f, 1.0f, val));
        repaint();
    };

    decayCurve.setType(ThumbType::Curve);
    decayCurve.onPositionChanged = [this, &apvts]()
    {
        if (isSyncingFromParams) return;
        const auto start = attack.getCenterPosition();
        const auto end = decaySustain.getCenterPosition();
        const auto y = decayCurve.getCenterPosition().getY();
        const auto curveT = (end.getY() != start.getY())
            ? juce::jmap(y, start.getY(), end.getY(), 0.0f, 1.0f)
            : 0.5f;
        const auto val = curveValueFromT(curveT);
        apvts.getParameter(ApvtsParams::DecayCurveId)->setValueNotifyingHost(juce::jlimit(0.0f, 1.0f, val));
        repaint();
    };

    releaseCurve.setType(ThumbType::Curve);
    releaseCurve.onPositionChanged = [this, &apvts]()
    {
        if (isSyncingFromParams) return;
        const auto start = decaySustain.getCenterPosition();
        const auto end = release.getCenterPosition();
        const auto y = releaseCurve.getCenterPosition().getY();
        const auto curveT = (end.getY() != start.getY())
            ? juce::jmap(y, start.getY(), end.getY(), 0.0f, 1.0f)
            : 0.5f;
        const auto val = curveValueFromT(curveT);
        apvts.getParameter(ApvtsParams::ReleaseCurveId)->setValueNotifyingHost(juce::jlimit(0.0f, 1.0f, val));
        repaint();
    };
}

GraphAdsr::~GraphAdsr() {}

float GraphAdsr::getDecaySustainMaxX() const
{
    return juce::jmax(attack.getCenterPosition().getX(), w);
}

void GraphAdsr::addCurve(juce::Path& path, juce::Point<float> start, juce::Point<float> end, float value) const
{
    constexpr int resolution = 64;
    const float exponent = curveExponent(value);

    for (int i = 1; i <= resolution; ++i)
    {
        const float t = static_cast<float>(i) / static_cast<float>(resolution);
        const float curvedT = std::pow(t, exponent);
        const float x = juce::jmap(t, 0.0f, 1.0f, start.getX(), end.getX());
        const float y = juce::jmap(curvedT, 0.0f, 1.0f, start.getY(), end.getY());
        path.lineTo(x, y);
    }
}

void GraphAdsr::syncCurveThumb(Thumb& curveThumb, juce::Point<float> start, juce::Point<float> end, const juce::String& paramId)
{
    const auto midX = (start.getX() + end.getX()) * 0.5f;
    const auto minY = juce::jmin(start.getY(), end.getY());
    const auto maxY = juce::jmax(start.getY(), end.getY());

    curveThumb.setLimit({midX, minY, 0.0f, maxY - minY});

    const auto val = apvts.getParameter(paramId)->getValue();
    const auto exponent = curveExponent(val);
    const auto curveT = std::pow(0.5f, exponent);
    const auto curveY = juce::jmap(curveT, 0.0f, 1.0f, start.getY(), end.getY());

    curveThumb.setCenterPosition({midX, curveY});
}

void GraphAdsr::paint(juce::Graphics& g)
{
    g.fillAll(Palette::borderDark);

    juce::Path envelope;

    const auto startPoint = juce::Point<float>(0.0f, h);
    const auto attackPoint = attack.getCenterPosition();
    const auto decaySustainPoint = decaySustain.getCenterPosition();
    const auto releasePoint = release.getCenterPosition();

    envelope.startNewSubPath(startPoint);

    const auto attackCurveValue = apvts.getParameter(ApvtsParams::AttackCurveId)->getValue();
    const auto decayCurveValue = apvts.getParameter(ApvtsParams::DecayCurveId)->getValue();
    const auto releaseCurveValue = apvts.getParameter(ApvtsParams::ReleaseCurveId)->getValue();

    addCurve(envelope, startPoint, attackPoint, attackCurveValue);
    addCurve(envelope, attackPoint, decaySustainPoint, decayCurveValue);
    addCurve(envelope, decaySustainPoint, releasePoint, releaseCurveValue);

    g.setColour(Palette::crimson);
    g.strokePath(envelope, juce::PathStrokeType(2.0f));

    juce::Path filledEnvelope(envelope);
    filledEnvelope.lineTo(releasePoint.getX(), h);
    filledEnvelope.lineTo(startPoint.getX(), h);
    filledEnvelope.closeSubPath();

    g.setColour(Palette::crimsonDark.withAlpha(0.5f));
    g.fillPath(filledEnvelope);
}

void GraphAdsr::resized()
{
    w = static_cast<float>(getWidth());
    h = static_cast<float>(getHeight());

    attack.setSize(thumbSize, thumbSize);
    decaySustain.setSize(thumbSize, thumbSize);
    release.setSize(thumbSize, thumbSize);
    attackCurve.setSize(thumbSize, thumbSize);
    decayCurve.setSize(thumbSize, thumbSize);
    releaseCurve.setSize(thumbSize, thumbSize);

    attack.setLimit({0.0f, 0.0f, w / 3.0f, 0.0f});
    decaySustain.setLimit({w / 3.0f, 0.0f, getDecaySustainMaxX() - w / 3.0f, h});
    release.setLimit({2.0f * w / 3.0f, h, w / 3.0f, 0.0f});
}

void GraphAdsr::timerCallback()
{
    isSyncingFromParams = true;

    const auto attackVal = apvts.getParameter(ApvtsParams::AttackId)->getValue();
    const auto attackX = juce::jmap(attackVal, 0.0f, 1.0f, 0.0f, w / 3.0f);

    attack.setLimit({0.0f, 0.0f, w / 3.0f, 0.0f});
    attack.setCenterPosition({attackX, 0.0f});

    const auto decayVal = apvts.getParameter(ApvtsParams::DecayId)->getValue();
    const auto sustainVal = apvts.getParameter(ApvtsParams::SustainId)->getValue();
    const auto decayMinX = attack.getCenterPosition().getX();
    const auto decayMaxX = getDecaySustainMaxX();

    decaySustain.setLimit({decayMinX, 0.0f, decayMaxX - decayMinX, h});

    const auto decayX = juce::jmap(decayVal, 0.0f, 1.0f, decayMinX, decayMaxX);
    const auto sustainY = juce::jmap(sustainVal, 0.0f, 1.0f, h, 0.0f);

    decaySustain.setCenterPosition({decayX, sustainY});

    const auto releaseVal = apvts.getParameter(ApvtsParams::ReleaseId)->getValue();
    const auto releaseMinX = decaySustain.getCenterPosition().getX();
    const auto releaseMaxX = w;

    release.setLimit({releaseMinX, h, juce::jmax(0.0f, releaseMaxX - releaseMinX), 0.0f});

    const auto releaseX = juce::jmap(releaseVal, 0.0f, 1.0f, releaseMinX, releaseMaxX);
    release.setCenterPosition({releaseX, h});

    syncCurveThumb(attackCurve, {0.0f, h}, attack.getCenterPosition(), ApvtsParams::AttackCurveId);
    syncCurveThumb(decayCurve, attack.getCenterPosition(), decaySustain.getCenterPosition(), ApvtsParams::DecayCurveId);
    syncCurveThumb(releaseCurve, decaySustain.getCenterPosition(), release.getCenterPosition(), ApvtsParams::ReleaseCurveId);

    repaint();
    isSyncingFromParams = false;
}