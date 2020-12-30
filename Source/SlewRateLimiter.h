#pragma once

#include <JuceHeader.h>

#define TINY 0.0000000001

/**

This is a slew rate limiter with adjustable attack and release time constants.
It works similar to the "EnvelopeFollower" class, except that it does not work
on rectified or squared signals but on the original bipolar signal. It is
useful to smooth discontinouities in control signals such as an LFO-output 
when the LFO produces a quare wave.

*/

class SlewRateLimiter  
{

public:

 //---------------------------------------------------------------------------
 // construction/destruction:

 SlewRateLimiter();
 ~SlewRateLimiter();

 //---------------------------------------------------------------------------
 // parameter settings:

    void setSampleRate (float newSampleRate);
 /**< Sets the sample-rate. */

 float getAttackTime() const { return attackTime; } ; 
 /**< Returns the attack-time in seconds */

    void setAttackTime (float newAttackTime); 
 /**< Sets the attack-time in seconds */

 float getReleaseTime() const { return releaseTime; } ; 
 /**< Returns the release-time in seconds */

    void setReleaseTime(float newReleaseTime);
 /**< Sets the release-time in seconds */

 //---------------------------------------------------------------------------
 // audio processing:

    inline float getSample(float in);

 //---------------------------------------------------------------------------
 // others:

    void reset();

protected:

 float y_1;	// previous ouput sample of the unit

    float coeffAttack, coeffRelease; 
     // the recursion coefficient for the recursive RC-Filter - the smoothing is 
  // performed via the recursive filter equation: 
  // y[n] = (1-coeff)*x[n] + coeff*y[n-1]. 
  // Which coeff of the two will be used is determined by the signal - if it 
  // is rising ( y[n-1] < x[n] ), the attack-coefficient is used, if it is 
  // falling, the release coeeficient will be used


    float sampleRate;
    float attackTime, releaseTime; // time constants in seconds
};

//----------------------------------------------------------------------------
// from here: definitions of the functions to be inlined, i.e. all functions
// which are supposed to be called at audio-rate (they can't be put into
// the .cpp file):

inline float SlewRateLimiter::getSample(float in)
{
    static float temp, coeff;

    temp = in;

    // decide, if signal is rising or falling and choose appropriate filter
 // coefficient:
    if( y_1 < temp )
   	 coeff = coeffAttack;
    else
   	 coeff = coeffRelease;

    // perform the envelope detection:
    temp = (1.0f-coeff)*temp 
        + coeff*y_1
        + TINY;

    // update the memorized output for next call:
    y_1 = temp;

    return temp;
}
