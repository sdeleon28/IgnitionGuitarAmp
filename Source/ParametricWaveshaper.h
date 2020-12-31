#pragma once

#include <JuceHeader.h>
#include "BaseWaveshaper.h"

class ParametricWaveshaper
{
public:
    ParametricWaveshaper();
    void prepare(const dsp::ProcessSpec& spec);
    float processSample(int channel, int sample, float x, AudioBuffer<float>* sidechainBuffer);
    void processWithSidechain (const dsp::ProcessContextReplacing<float>& context, AudioBuffer<float>* sidechainBuffer) noexcept;
};
