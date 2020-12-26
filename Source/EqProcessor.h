/*
  ==============================================================================

    This is the Frequalizer processor

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//==============================================================================
/**
*/
class EqProcessor
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

    static String getBandID (size_t index);

    //==============================================================================
    EqProcessor();
    ~EqProcessor();

    //==============================================================================
    void reset() noexcept;
    void prepare (const dsp::ProcessSpec& spec) noexcept;
    void releaseResources();

    void process (const dsp::ProcessContextReplacing<float>& context) noexcept;

    size_t getNumBands () const;

    String getBandName   (size_t index) const;

    void setBandSolo (int index);
    bool getBandSolo (int index) const;

    static StringArray getFilterTypeNames();

    const std::vector<double>& getMagnitudes ();

    //==============================================================================
    const String getName() const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    bool isMidiEffect() const;
    double getTailLengthSeconds() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    struct Band {
        Band (const String& nameToUse, FilterType typeToUse,
            float frequencyToUse, float qualityToUse, float gainToUse=1.0f, bool shouldBeActive=true)
          : name (nameToUse),
            type (typeToUse),
            frequency (frequencyToUse),
            quality (qualityToUse),
            gain (gainToUse),
            active (shouldBeActive)
        {}

        String      name;
        FilterType  type      = BandPass;
        float       frequency = 1000.0f;
        float       quality   = 1.0f;
        float       gain      = 1.0f;
        bool        active    = true;
        std::vector<double> magnitudes;
    };

    // BEGIN adapter code
    void setGain(float newValueInDb);
    void setBand(int index, const Band& newBand);
    // END adapter code

    Band* getBand (size_t index);
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqProcessor)

    void updateBand (const size_t index);

    void updateBypassedStates ();

    std::vector<Band>    bands;

    std::vector<double> frequencies;
    std::vector<double> magnitudes;

    bool wasBypassed = true;

    using FilterBand = dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>>;
    using Gain       = dsp::Gain<float>;
    dsp::ProcessorChain<FilterBand, FilterBand, FilterBand, FilterBand, FilterBand, FilterBand, Gain> filter;

    double sampleRate = 0;

    int soloed = -1;
};
