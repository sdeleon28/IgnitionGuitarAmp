#include "ParametricWaveshaper.h"

ParametricWaveshaper::ParametricWaveshaper()
{
}

void ParametricWaveshaper::setParameter(float parameterToUse)
{
    parameter = parameterToUse;
}

float ParametricWaveshaper::processSample(float x)
{
    return (x*(abs(x) + parameter)/(x*x + (parameter-1) * abs(x) + 1)) * 0.7;
}
