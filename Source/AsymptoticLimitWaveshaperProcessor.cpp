#include "AsymptoticLimitWaveshaperProcessor.h"

AsymptoticLimitWaveshaperProcessor::AsymptoticLimitWaveshaperProcessor()
{
}

float AsymptoticLimitWaveshaperProcessor::processSample(float sample)
{
    return sample / (std::abs(sample) + 1);
}
