#include "AsymptoticLimitWaveshaperProcessor.h"

AsymptoticLimitWaveshaperProcessor::AsymptoticLimitWaveshaperProcessor ()
{
}

void AsymptoticLimitWaveshaperProcessor::prepare (const dsp::ProcessSpec& spec) noexcept
{
}

void AsymptoticLimitWaveshaperProcessor::reset() noexcept
{
}

void AsymptoticLimitWaveshaperProcessor::process (const dsp::ProcessContextReplacing<float>& context) noexcept
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
            auto processedSample = src[sample] / (std::abs(src[sample]) + 1);
            dst[sample] = jlimit(-1.f, 1.f, processedSample);
        }
    }
}
