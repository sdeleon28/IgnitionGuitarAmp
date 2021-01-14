#include "DialLookAndFeel.h"

void DialLookAndFeel::drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider &slider)
{
    float diameter = jmin(width, height) * 0.8;
    float radius = diameter / 2;
    float centreX = x + width / 2;
    float centreY = x + height / 2;
    float rx = centreX - radius;
    float ry = centreY - radius;
    float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

    Rectangle<float> dialArea(rx, ry, diameter, diameter);

    g.setColour(Colours::red);
    g.fillEllipse(dialArea);

    g.setColour(Colours::black);

    Path dialTick;
    dialTick.addRectangle(0, -radius, 10.0f, radius * 0.33f);

    g.fillPath(dialTick, AffineTransform::rotation(angle).translated(centreX, centreY));

    g.drawEllipse(rx, ry, diameter, diameter, 5.0f);
}
