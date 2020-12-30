#pragma once

#include <JuceHeader.h>
#include "BaseWaveshaper.h"

class SlewLimiter : public BaseWaveshaper
{
public:
    SlewLimiter();
    float processSample(float sample) override;
private:
    // User parameters
    float rise = 0.5f;
    float fall = 0.999f;

    // Constants
    float slewMin = 0.1f;	// Minimum slope in volts per second
    float slewMax = 10000.f; // Maximum slope in volts per second

    // Internal variables
    float Ts = 1.f / sampleRate;
    float slewRise = slewMax * Ts * std::pow (slewMin / slewMax, rise);
    float slewFall = slewMax * Ts * std::pow (slewMin / slewMax, fall);

    // State variables
    float out = 0.f;
};
