#pragma once
#include <JuceHeader.h>

class BaseWaveshaper
{
public:
    virtual ~BaseWaveshaper();
    void prepare (const dsp::ProcessSpec& spec) noexcept;
    void reset() noexcept;
    virtual float processSample(float sample);
    void process (const dsp::ProcessContextReplacing<float>& context) noexcept;
};
