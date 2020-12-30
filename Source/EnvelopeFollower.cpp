#include "EnvelopeFollower.h"

EnvelopeFollower::EnvelopeFollower()
{
  mode = 2;
}

EnvelopeFollower::~EnvelopeFollower()
{
}

void EnvelopeFollower::prepare (const dsp::ProcessSpec& spec) noexcept
{
    setSampleRate(spec.sampleRate);
    reset();
}

// FIXME: Duplicated code
void EnvelopeFollower::process (const dsp::ProcessContextReplacing<float>& context) noexcept
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
            dst[sample] = jlimit(-1.f, 1.f, getSample(src[sample]));
        }
    }
}

void EnvelopeFollower::setMode(int Mode)
{
  if( Mode >= MEAN_ABS && Mode < NUM_MODES )
    mode = Mode;
}
