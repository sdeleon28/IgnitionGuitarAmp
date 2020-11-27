#include <JuceHeader.h>

class WaveshaperProcessor
{
public:
    WaveshaperProcessor();
    void setGain(float newGain) noexcept;
    float getGain() const noexcept;
    void setOutLevel(float newOutLevel) noexcept;
    float getOutLevel() const noexcept;
    //void setWaveshaperType(WaveshaperType newWaveshaperType) noexcept;
    //WaveshaperType getWaveshaperType() const noexcept;

    void prepare (const dsp::ProcessSpec& spec) noexcept;
    void reset() noexcept;
    void process (const dsp::ProcessContextReplacing<float>& context) noexcept;
    enum WaveshaperType { hyperbolicTangent = 1, square, sinewave };
private:
    WaveshaperType waveshaperType { WaveshaperType::hyperbolicTangent };
    float gain { 0.5f };
    float outLevel { 1.f };
};
