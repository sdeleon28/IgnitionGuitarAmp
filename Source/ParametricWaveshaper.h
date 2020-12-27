#include <JuceHeader.h>
#include "BaseWaveshaper.h"

class ParametricWaveshaper : public BaseWaveshaper
{
public:
    ParametricWaveshaper();
    void setParameter(float parameterToUse);
    float processSample(float sample) override;
private:
    float parameter;
};
