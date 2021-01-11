#include "ParametricWaveshaper.h"

ParametricWaveshaper::ParametricWaveshaper()
{
}

void ParametricWaveshaper::prepare(const dsp::ProcessSpec &spec)
{
}

float ParametricWaveshaper::processSample(int channel, int sample, float x, AudioBuffer<float> *sidechainBuffer)
{
    float parameter = sidechainBuffer->getSample(channel, sample);
    return (x * (abs(x) + parameter) / (x * x + (parameter - 1) * abs(x) + 1)) * 0.7;
}

// TODO: Duplicated code
void ParametricWaveshaper::processWithSidechain(
    const dsp::ProcessContextReplacing<float> &context, AudioBuffer<float> *sidechainBuffer) noexcept
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
            dst[sample] = jlimit(-1.f, 1.f, processSample(channel, sample, src[sample], sidechainBuffer));
        }
    }
}
