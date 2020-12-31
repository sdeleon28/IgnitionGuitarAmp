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
void EnvelopeFollower::process (const dsp::ProcessContextNonReplacing<float>& context) noexcept
{
    auto&& inBlock  = context.getInputBlock();
    auto&& outBlock = context.getOutputBlock();

    jassert (inBlock.getNumChannels() == outBlock.getNumChannels());

    // I don't necessarily need to know the exact sample count for the buffer I use here
    // (as long as it's long enough). If this were outputting samples for use as plugin output
    // I would be in trouble, but this is just the sidechain for another plugin.
    // BE AWARE of that, and refactor if you need this to behave like a regular processor.
    //jassert (inBlock.getNumSamples() == outBlock.getNumSamples());

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
