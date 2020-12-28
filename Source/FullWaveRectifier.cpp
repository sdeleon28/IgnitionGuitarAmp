#include "FullWaveRectifier.h"

FullWaveRectifier::FullWaveRectifier()
{
}

float FullWaveRectifier::processSample(float sample)
{
    return std::abs(sample);
}
