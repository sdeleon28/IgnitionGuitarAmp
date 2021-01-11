#include "BaseWaveshaper.h"

BaseWaveshaper::~BaseWaveshaper()
{
}

void BaseWaveshaper::prepare(const dsp::ProcessSpec &spec) noexcept
{
    sampleRate = spec.sampleRate;
}

void BaseWaveshaper::reset() noexcept
{
}

float BaseWaveshaper::processSample(float x)
{
    return x;
}

void BaseWaveshaper::process(const dsp::ProcessContextReplacing<float> &context) noexcept
{
    auto &&inBlock = context.getInputBlock();
    auto &&outBlock = context.getOutputBlock();

    jassert(inBlock.getNumChannels() == outBlock.getNumChannels());
    jassert(inBlock.getNumSamples() == outBlock.getNumSamples());

    auto len = inBlock.getNumSamples();
    auto numChannels = inBlock.getNumChannels();

    if (context.isBypassed)
    {
        if (context.usesSeparateInputAndOutputBlocks())
            outBlock.copyFrom(inBlock);
        return;
    }

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto *src = inBlock.getChannelPointer(channel);
        auto *dst = outBlock.getChannelPointer(channel);
        for (int sample = 0; sample < len; ++sample)
        {
            dst[sample] = jlimit(-1.f, 1.f, processSample(src[sample]));
        }
    }
}
