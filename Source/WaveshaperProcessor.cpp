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


void WaveshaperProcessor::setWaveshaperType(WaveshaperType newWaveshaperType) noexcept
{
    waveshaperType = newWaveshaperType;
}

WaveshaperType WaveshaperProcessor::getWaveshaperType() const noexcept
{
    return waveshaperType;
}

void WaveshaperProcessor::prepare (const dsp::ProcessSpec& spec) noexcept
{
    reset();
}

void WaveshaperProcessor::reset() noexcept
{
    gain = 0.5f;
    outLevel = 1.f;
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
            float processedSample = src[sample]; // Default to same sample
            if (waveshaperType == WaveshaperType::asymptoticLimit)
            {
                auto gainedSample = src[sample] * gain;
                processedSample = gainedSample / (std::abs(gainedSample) + 1);
            }
            else if (waveshaperType == WaveshaperType::hyperbolicTangent)
            {
                processedSample = std::tanh(src[sample] * gain);
            }
            else if (waveshaperType == WaveshaperType::square)
            {
                processedSample = (src[sample] >= 0 ? 1.f : -1.f);
            }
            else if (waveshaperType == WaveshaperType::sinewave)
            {
                processedSample = std::sin(src[sample] * gain);
            }
            dst[sample] = jlimit(-1.f, 1.f, processedSample) * outLevel;
        }
    }
}
