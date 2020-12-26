#include <JuceHeader.h>
#include "BaseWaveshaper.h"

class ParametricWaveshaper : public BaseWaveshaper
{
public:
    ParametricWaveshaper();
    float processSample(float sample) override;
};
