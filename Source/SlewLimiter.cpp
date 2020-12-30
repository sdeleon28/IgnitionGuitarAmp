#include "SlewLimiter.h"

SlewLimiter::SlewLimiter()
{
}

float SlewLimiter::processSample(float sample)
{
    // Rise limiting
    if (sample > out)
        out = jmin (sample, out + slewRise);
    // Fall limiting
    else
        out = jmax (sample, out - slewFall);
    return out;
}
