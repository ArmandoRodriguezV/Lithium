#include "Thumb.h"
#include "../../Palette/Palette.h"

void Thumb::paint(juce::Graphics& g) {
    g.setColour(Palette::crimsonLight);

    if (type == ThumbType::Simple) {
        g.fillEllipse(0.0f, 0.0f, getWidth(), getHeight());
    }

    if (type == ThumbType::Curve) {
        const auto w = getWidth();
        const auto h = getHeight();
        const auto pad = 10.0f;

        g.drawEllipse(0.0f, 0.0f, getWidth(), getHeight(), 2.0f);
        g.fillEllipse(pad, pad, w - pad * 2, h - pad * 2);
    }
}

void Thumb::mouseDown(const juce::MouseEvent &event)
{
    dragger.startDraggingComponent(this, event);
}

void Thumb::mouseDrag(const juce::MouseEvent& event)
{
    dragger.dragComponent(this, event, nullptr);

    const float halfWidth = getWidth() * 0.5f;
    const float halfHeight = getHeight() * 0.5f;

    const float centerX = static_cast<float>(getX()) + halfWidth;
    const float centerY = static_cast<float>(getY()) + halfHeight;

    setCenterPosition({ centerX, centerY });
}

void Thumb::setType(ThumbType newType)
{
    type = newType;
}

juce::Point<float> Thumb::getCenterPosition() const
{
    return position;
}

void Thumb::setLimit(juce::Rectangle<float> newLimit)
{
    limit = newLimit;
}

void Thumb::setCenterPosition(juce::Point<float> newPosition)
{
    const float limitedX = juce::jlimit(limit.getX(), limit.getRight(), newPosition.getX());
    const float limitedY = juce::jlimit(limit.getY(), limit.getBottom(), newPosition.getY());

    setCentrePosition(static_cast<int>(limitedX), static_cast<int>(limitedY));

    position.setXY(limitedX, limitedY);

    if (onPositionChanged)
        onPositionChanged();
}