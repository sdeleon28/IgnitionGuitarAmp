#include "ParametricWaveshaper.h"

ParametricWaveshaper::ParametricWaveshaper()
{
}

float ParametricWaveshaper::processSample(float x)
{
    float a = 0.2833f;
    return (x*(abs(x) + a)/(x*x + (a-1) * abs(x) + 1)) * 0.7;
}
