#include "DynamicWaveshaper.h"

DynamicWaveshaper::~DynamicWaveshaper()
{
    delete envOutputBuffer;
}

void DynamicWaveshaper::prepare (const dsp::ProcessSpec& spec) noexcept
{
    sampleRate = spec.sampleRate;
    maximumBlockSize = spec.maximumBlockSize;
    numChannels = spec.numChannels;
    envOutputBuffer = new AudioBuffer<float>(numChannels, maximumBlockSize);
    envOutputBlock = std::make_unique<dsp::AudioBlock<float>>(*envOutputBuffer);
    envelopeFollower.prepare(spec);
    parametricWaveshaper.prepare(spec);
}

void DynamicWaveshaper::setAttackTime(float attackTime)
{
    envelopeFollower.setAttackTime(attackTime);
}

void DynamicWaveshaper::setReleaseTime(float releaseTime)
{
    envelopeFollower.setReleaseTime(releaseTime);
}

void DynamicWaveshaper::reset() noexcept
{
}

void DynamicWaveshaper::process (const dsp::ProcessContextReplacing<float>& context) noexcept
{
    dsp::ProcessContextNonReplacing<float> envContext(
        context.getInputBlock(),
        *envOutputBlock
    );
    envelopeFollower.process(envContext);
    parametricWaveshaper.processWithSidechain(context, envOutputBuffer);
}
