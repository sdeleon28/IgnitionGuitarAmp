#pragma once
#include <JuceHeader.h>

class SingleEqBandProcessor
{
  public:
    enum FilterType
    {
        NoFilter = 0,
        HighPass,
        HighPass1st,
        LowShelf,
        BandPass,
        AllPass,
        AllPass1st,
        Notch,
        Peak,
        HighShelf,
        LowPass1st,
        LowPass,
        LastFilterID
    };

    SingleEqBandProcessor();
    ~SingleEqBandProcessor();

    void reset() noexcept;
    void setPluginProcessor(AudioProcessor *processorToUse) noexcept;
    void prepare(const dsp::ProcessSpec &spec) noexcept;
    void releaseResources();
    void process(const dsp::ProcessContextReplacing<float> &context) noexcept;

    struct Band
    {
        Band(
            const String &nameToUse,
            FilterType typeToUse,
            float frequencyToUse,
            float qualityToUse,
            float gainToUse = 1.0f,
            bool shouldBeActive = true)
            : name(nameToUse)
            , type(typeToUse)
            , frequency(frequencyToUse)
            , quality(qualityToUse)
            , gain(gainToUse)
            , active(shouldBeActive)
        {
        }

        String name;
        FilterType type = BandPass;
        float frequency = 1000.0f;
        float quality = 1.0f;
        float gain = 1.0f;
        bool active = true;
        std::vector<double> magnitudes;
    };

    void setBandGain(float gainToUse);
    void setBand(const Band &newBand);
    SingleEqBandProcessor::Band *getBand();

  private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SingleEqBandProcessor)

    void updateBand();
    Band band;
    using FilterBand = dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>>;
    FilterBand filter;
    double sampleRate = 0;
    AudioProcessor *pluginProcessor = nullptr;
};
