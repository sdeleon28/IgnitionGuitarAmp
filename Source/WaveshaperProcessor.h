#include <JuceHeader.h>

enum WaveshaperType { asymptoticLimit = 1, hyperbolicTangent, square, sinewave };

class WaveshaperProcessor
{
public:
    WaveshaperProcessor();
    void setGain(float newGain) noexcept;
    float getGain() const noexcept;
    void setOutLevel(float newOutLevel) noexcept;
    float getOutLevel() const noexcept;
    
    void setWaveshaperType(WaveshaperType newWt) noexcept;
    WaveshaperType getWaveshaperType() const noexcept;

    //void setWaveshaperType(WaveshaperType newWaveshaperType) noexcept;
    //WaveshaperType getWaveshaperType() const noexcept;

    void prepare (const dsp::ProcessSpec& spec) noexcept;
    void reset() noexcept;
    void process (const dsp::ProcessContextReplacing<float>& context) noexcept;
private:
    WaveshaperType waveshaperType { WaveshaperType::asymptoticLimit };
    float gain { 82.f };
    float outLevel { .15f };
};
