#pragma once

#include <JuceHeader.h>
#include "BaseWaveshaper.h"

class AsymptoticLimitWaveshaperProcessor : public BaseWaveshaper
{
  public:
    AsymptoticLimitWaveshaperProcessor();
    float processSample(float sample) override;
};
