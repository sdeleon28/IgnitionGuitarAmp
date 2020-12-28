#pragma once

#include <JuceHeader.h>
#include "BaseWaveshaper.h"

class FullWaveRectifier : public BaseWaveshaper
{
public:
    FullWaveRectifier();
    float processSample(float sample) override;
};
