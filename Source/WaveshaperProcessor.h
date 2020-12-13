#include <JuceHeader.h>

class WaveshaperProcessor
{
public:
    WaveshaperProcessor();
    void setGain(float newGain) noexcept;
    float getGain() const noexcept;
    void setOutLevel(float newOutLevel) noexcept;
    float getOutLevel() const noexcept;
    void prepare (const dsp::ProcessSpec& spec) noexcept;
    void reset() noexcept;
    void process (const dsp::ProcessContextReplacing<float>& context) noexcept;
private:
    float gain { 82.f };
    float outLevel { .15f };
};
