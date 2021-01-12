#include "SlewRateLimiter.h"
SlewRateLimiter::SlewRateLimiter()
{
    // parameter initializition:
    sampleRate = 48000.0f; // default sample rate
                           // These default values give me a decent envelope shape for a clean
                           // guitar waveform
    attackTime = 0.01f;
    releaseTime = 0.1f;

    // calculation of internal variables:
    coeffAttack = 0.0f;
    coeffRelease = 0.0f;

    // init previous output sample:
    y_1 = 0.0f;
}

SlewRateLimiter::~SlewRateLimiter()
{
}

//----------------------------------------------------------------------------
// parameter settings:

void SlewRateLimiter::setSampleRate(float newSampleRate)
{
    if (newSampleRate > 0.01f)
        sampleRate = newSampleRate;

    setAttackTime(attackTime);
    setReleaseTime(releaseTime);

    // coeffAttack  = exp( -1.0f / (sampleRate*attackTime)  );
    // coeffRelease = exp( -1.0f / (sampleRate*releaseTime) );
}

void SlewRateLimiter::setAttackTime(float newAttackTime)
{
    if (newAttackTime > 0.0f)
    {
        attackTime = newAttackTime;
        coeffAttack = exp(-1.0f / (sampleRate * attackTime));
    }
    else
    {
        attackTime = 0.0f;
        coeffAttack = 0.0f;
        ;
    }
}

void SlewRateLimiter::setReleaseTime(float newReleaseTime)
{
    if (newReleaseTime > 0.0f)
    {
        releaseTime = newReleaseTime;
        coeffRelease = exp(-1.0f / (sampleRate * releaseTime));
    }
    else
    {
        releaseTime = 0.0f;
        coeffRelease = 0.0f;
    }
}

//----------------------------------------------------------------------------
// others:

void SlewRateLimiter::reset()
{
    y_1 = 0;
}
