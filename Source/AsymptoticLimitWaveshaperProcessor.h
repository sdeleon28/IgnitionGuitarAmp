#include <JuceHeader.h>

class AsymptoticLimitWaveshaperProcessor
{
public:
    AsymptoticLimitWaveshaperProcessor();
    void prepare (const dsp::ProcessSpec& spec) noexcept;
    void reset() noexcept;
    void process (const dsp::ProcessContextReplacing<float>& context) noexcept;
};
