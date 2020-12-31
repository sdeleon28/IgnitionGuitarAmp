#pragma once

#include <JuceHeader.h>
#include "EnvelopeFollower.h"
#include "ParametricWaveshaper.h"

class DynamicWaveshaper
{
public:
    virtual ~DynamicWaveshaper();
    virtual void prepare (const dsp::ProcessSpec& spec) noexcept;
    void setAttackTime(float attackTime);
    void setReleaseTime(float releaseTime);
    void reset() noexcept;
    void process (const dsp::ProcessContextReplacing<float>& context) noexcept;
protected:
    float sampleRate;
    int maximumBlockSize;
    int numChannels;
    AudioBuffer<float>* envOutputBuffer;
    std::unique_ptr<dsp::AudioBlock<float>> envOutputBlock;
    EnvelopeFollower envelopeFollower;
    ParametricWaveshaper parametricWaveshaper;
};
