#include "WaveshaperProcessor.h"

WaveshaperProcessor::WaveshaperProcessor ()
{
    reset();
}

float WaveshaperProcessor::getGain() const noexcept
{
    return gain;
}

void WaveshaperProcessor::setGain(float newGain) noexcept
{
    gain = newGain;
}

float WaveshaperProcessor::getOutLevel() const noexcept
{
    return outLevel;
}

void WaveshaperProcessor::setOutLevel(float newOutLevel) noexcept
{
    outLevel = newOutLevel;
}


void WaveshaperProcessor::prepare (const dsp::ProcessSpec& spec) noexcept
{
    reset();
}

void WaveshaperProcessor::reset() noexcept
{
    gain = 82.0f;
    outLevel = .15f;
}

void WaveshaperProcessor::process (const dsp::ProcessContextReplacing<float>& context) noexcept
{
    auto&& inBlock  = context.getInputBlock();
    auto&& outBlock = context.getOutputBlock();

    jassert (inBlock.getNumChannels() == outBlock.getNumChannels());
    jassert (inBlock.getNumSamples() == outBlock.getNumSamples());

    auto len         = inBlock.getNumSamples();
    auto numChannels = inBlock.getNumChannels();

    if (context.isBypassed)
    {
        if (context.usesSeparateInputAndOutputBlocks())
            outBlock.copyFrom (inBlock);
        return;
    }

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* src = inBlock.getChannelPointer (channel);
        auto* dst = outBlock.getChannelPointer (channel);
        for (int sample = 0; sample < len; ++sample)
        {
            auto gainedSample = src[sample] * gain;
            auto processedSample = gainedSample / (std::abs(gainedSample) + 1);
            dst[sample] = jlimit(-1.f, 1.f, processedSample) * outLevel;
        }
    }
}
