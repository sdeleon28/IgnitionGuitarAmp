#pragma once
#include <JuceHeader.h>

class DialLookAndFeel : public LookAndFeel_V4
{
public:
    void drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider &slider) override;
};
