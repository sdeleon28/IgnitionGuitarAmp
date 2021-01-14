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
    auto rotation = AffineTransform::rotation(angle, centreX, centreY);
    // TODO: Move this to a member variable?
    auto image = ImageCache::getFromMemory(BinaryData::dial_png, BinaryData::dial_pngSize);
    g.addTransform(rotation);
    g.drawImage(image, dialArea);
}
