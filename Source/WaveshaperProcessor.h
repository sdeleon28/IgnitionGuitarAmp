#include <JuceHeader.h>

class WaveshaperProcessor
{
public:
    WaveshaperProcessor();
    void prepare (const dsp::ProcessSpec& spec) noexcept;
    void reset() noexcept;
    void process (const dsp::ProcessContextReplacing<float>& context) noexcept;
};
