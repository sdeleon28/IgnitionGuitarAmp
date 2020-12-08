/*
  ==============================================================================

    This is the Frequalizer implementation

  ==============================================================================
*/

#include "Analyser.h"
#include "EqProcessor.h"

String EqProcessor::paramOutput   ("output");
String EqProcessor::paramType     ("type");
String EqProcessor::paramFrequency("frequency");
String EqProcessor::paramQuality  ("quality");
String EqProcessor::paramGain     ("gain");
String EqProcessor::paramActive   ("active");

String EqProcessor::getBandID (size_t index)
{
    switch (index)
    {
        case 0: return "Lowest";
        case 1: return "Low";
        case 2: return "Low Mids";
        case 3: return "High Mids";
        case 4: return "High";
        case 5: return "Highest";
        default: break;
    }
    return "unknown";
}

int EqProcessor::getBandIndexFromID (String paramID)
{
    for (size_t i=0; i < 6; ++i)
        if (paramID.startsWith (getBandID (i) + "-"))
            return int (i);

    return -1;
}

std::vector<EqProcessor::Band> createDefaultBands()
{
    std::vector<EqProcessor::Band> defaults;
    defaults.push_back (EqProcessor::Band (TRANS ("Lowest"),    Colours::blue,   EqProcessor::HighPass,    20.0f, 0.707f));
    defaults.push_back (EqProcessor::Band (TRANS ("Low"),       Colours::brown,  EqProcessor::LowShelf,   250.0f, 0.707f));
    defaults.push_back (EqProcessor::Band (TRANS ("Low Mids"),  Colours::green,  EqProcessor::Peak,       500.0f, 0.707f));
    defaults.push_back (EqProcessor::Band (TRANS ("High Mids"), Colours::coral,  EqProcessor::Peak,      1000.0f, 0.707f));
    defaults.push_back (EqProcessor::Band (TRANS ("High"),      Colours::orange, EqProcessor::HighShelf, 5000.0f, 0.707f));
    defaults.push_back (EqProcessor::Band (TRANS ("Highest"),   Colours::red,    EqProcessor::LowPass,  12000.0f, 0.707f));
    return defaults;
}

AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<AudioProcessorParameterGroup>> params;

    // setting defaults
    const float maxGain = Decibels::decibelsToGain (24.0f);
    auto defaults = createDefaultBands();

    {
        auto param = std::make_unique<AudioParameterFloat> (EqProcessor::paramOutput, TRANS ("Output"),
                                                            NormalisableRange<float> (0.0f, 2.0f, 0.01f), 1.0f,
                                                            TRANS ("Output level"),
                                                            AudioProcessorParameter::genericParameter,
                                                            [](float value, int) {return String (Decibels::gainToDecibels(value), 1) + " dB";},
                                                            [](String text) {return Decibels::decibelsToGain (text.dropLastCharacters (3).getFloatValue());});

        auto group = std::make_unique<AudioProcessorParameterGroup> ("global", TRANS ("Globals"), "|", std::move (param));
        params.push_back (std::move (group));
    }

    for (size_t i = 0; i < defaults.size(); ++i)
    {
        auto prefix = "Q" + String (i + 1) + ": ";

        auto typeParameter = std::make_unique<AudioParameterChoice> (EqProcessor::getTypeParamName (i),
                                                                     prefix + TRANS ("Filter Type"),
                                                                     EqProcessor::getFilterTypeNames(),
                                                                     defaults [i].type);

        auto freqParameter = std::make_unique<AudioParameterFloat> (EqProcessor::getFrequencyParamName (i),
                                                                    prefix + TRANS ("Frequency"),
                                                                    NormalisableRange<float> {20.0f, 20000.0f, 1.0f, std::log (0.5f) / std::log (980.0f / 19980.0f)},
                                                                    defaults [i].frequency,
                                                                    String(),
                                                                    AudioProcessorParameter::genericParameter,
                                                                    [](float value, int) { return (value < 1000) ?
                                                                        String (value, 0) + " Hz" :
                                                                        String (value / 1000.0, 2) + " kHz"; },
                                                                    [](String text) { return text.endsWith(" kHz") ?
                                                                        text.dropLastCharacters (4).getFloatValue() * 1000.0 :
                                                                        text.dropLastCharacters (3).getFloatValue(); });

        auto qltyParameter = std::make_unique<AudioParameterFloat> (EqProcessor::getQualityParamName (i),
                                                                    prefix + TRANS ("Quality"),
                                                                    NormalisableRange<float> {0.1f, 10.0f, 1.0f, std::log (0.5f) / std::log (0.9f / 9.9f)},
                                                                    defaults [i].quality,
                                                                    String(),
                                                                    AudioProcessorParameter::genericParameter,
                                                                    [](float value, int) { return String (value, 1); },
                                                                    [](const String& text) { return text.getFloatValue(); });

        auto gainParameter = std::make_unique<AudioParameterFloat> (EqProcessor::getGainParamName (i),
                                                                    prefix + TRANS ("Gain"),
                                                                    NormalisableRange<float> {1.0f / maxGain, maxGain, 0.001f,
                                                                        std::log (0.5f) / std::log ((1.0f - (1.0f / maxGain)) / (maxGain - (1.0f / maxGain)))},
                                                                    defaults [i].gain,
                                                                    String(),
                                                                    AudioProcessorParameter::genericParameter,
                                                                    [](float value, int) {return String (Decibels::gainToDecibels(value), 1) + " dB";},
                                                                    [](String text) {return Decibels::decibelsToGain (text.dropLastCharacters (3).getFloatValue());});

        auto actvParameter = std::make_unique<AudioParameterBool> (EqProcessor::getActiveParamName (i),
                                                                   prefix + TRANS ("Active"),
                                                                   defaults [i].active,
                                                                   String(),
                                                                   [](float value, int) {return value > 0.5f ? TRANS ("active") : TRANS ("bypassed");},
                                                                   [](String text) {return text == TRANS ("active");});

        auto group = std::make_unique<AudioProcessorParameterGroup> ("band" + String (i), defaults [i].name, "|",
                                                                     std::move (typeParameter),
                                                                     std::move (freqParameter),
                                                                     std::move (qltyParameter),
                                                                     std::move (gainParameter),
                                                                     std::move (actvParameter));

        params.push_back (std::move (group));
    }

    return { params.begin(), params.end() };
}

//==============================================================================
EqProcessor::EqProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor (BusesProperties()
                    .withInput  ("Input",  AudioChannelSet::stereo(), true)
                    .withOutput ("Output", AudioChannelSet::stereo(), true)
                    ),
#endif
state (*this, &undo, "PARAMS", createParameterLayout())
{
    frequencies.resize (300);
    for (size_t i=0; i < frequencies.size(); ++i) {
        frequencies [i] = 20.0 * std::pow (2.0, i / 30.0);
    }
    magnitudes.resize (frequencies.size());

    // needs to be in sync with the ProcessorChain filter
    bands = createDefaultBands();

    for (size_t i = 0; i < bands.size(); ++i)
    {
        bands [i].magnitudes.resize (frequencies.size(), 1.0);

        state.addParameterListener (getTypeParamName (i), this);
        state.addParameterListener (getFrequencyParamName (i), this);
        state.addParameterListener (getQualityParamName (i), this);
        state.addParameterListener (getGainParamName (i), this);
        state.addParameterListener (getActiveParamName (i), this);
    }

    state.addParameterListener (paramOutput, this);

    state.state = ValueTree (JucePlugin_Name);
}

EqProcessor::~EqProcessor()
{
    inputAnalyser.stopThread (1000);
    outputAnalyser.stopThread (1000);
}

//==============================================================================
const String EqProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EqProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EqProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EqProcessor::isMidiEffect() const
{
    return false;
}

double EqProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EqProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EqProcessor::getCurrentProgram()
{
    return 0;
}

void EqProcessor::setCurrentProgram (int)
{
}

const String EqProcessor::getProgramName (int)
{
    return {};
}

void EqProcessor::changeProgramName (int, const String&)
{
}

//==============================================================================
void EqProcessor::prepareToPlay (double newSampleRate, int newSamplesPerBlock)
{
    sampleRate = newSampleRate;

    dsp::ProcessSpec spec;
    spec.sampleRate = newSampleRate;
    spec.maximumBlockSize = uint32 (newSamplesPerBlock);
    spec.numChannels = uint32 (getTotalNumOutputChannels ());

    for (size_t i=0; i < bands.size(); ++i) {
        updateBand (i);
    }
    filter.get<6>().setGainLinear (*state.getRawParameterValue (paramOutput));

    filter.prepare (spec);

    inputAnalyser.setupAnalyser  (int (sampleRate), float (sampleRate));
    outputAnalyser.setupAnalyser (int (sampleRate), float (sampleRate));
}

void EqProcessor::releaseResources()
{
    inputAnalyser.stopThread (1000);
    outputAnalyser.stopThread (1000);
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EqProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // This checks if the input layout matches the output layout
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}
#endif

void EqProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    ignoreUnused (midiMessages);

    if (getActiveEditor() != nullptr)
        inputAnalyser.addAudioData (buffer, 0, getTotalNumInputChannels());

    if (wasBypassed) {
        filter.reset();
        wasBypassed = false;
    }
    dsp::AudioBlock<float>              ioBuffer (buffer);
    dsp::ProcessContextReplacing<float> context  (ioBuffer);
    filter.process (context);

    if (getActiveEditor() != nullptr)
        outputAnalyser.addAudioData (buffer, 0, getTotalNumOutputChannels());
}

AudioProcessorValueTreeState& EqProcessor::getPluginState()
{
    return state;
}

String EqProcessor::getTypeParamName (size_t index)
{
    return getBandID (index) + "-" + paramType;
}

String EqProcessor::getFrequencyParamName (size_t index)
{
    return getBandID (index) + "-" + paramFrequency;
}

String EqProcessor::getQualityParamName (size_t index)
{
    return getBandID (index) + "-" + paramQuality;
}

String EqProcessor::getGainParamName (size_t index)
{
    return getBandID (index) + "-" + paramGain;
}

String EqProcessor::getActiveParamName (size_t index)
{
    return getBandID (index) + "-" + paramActive;
}

void EqProcessor::parameterChanged (const String& parameter, float newValue)
{
    if (parameter == paramOutput) {
        filter.get<6>().setGainLinear (newValue);
        return;
    }

    int index = getBandIndexFromID (parameter);
    if (isPositiveAndBelow (index, bands.size()))
    {
        auto* band = getBand (size_t (index));
        if (parameter.endsWith (paramType)) {
            band->type = static_cast<FilterType> (static_cast<int> (newValue));
        }
        else if (parameter.endsWith (paramFrequency)) {
            band->frequency = newValue;
        }
        else if (parameter.endsWith (paramQuality)) {
            band->quality = newValue;
        }
        else if (parameter.endsWith (paramGain)) {
            band->gain = newValue;
        }
        else if (parameter.endsWith (paramActive)) {
            band->active = newValue >= 0.5f;
        }

        updateBand (size_t (index));
    }
}

size_t EqProcessor::getNumBands () const
{
    return bands.size();
}

String EqProcessor::getBandName   (size_t index) const
{
    if (isPositiveAndBelow (index, bands.size()))
        return bands [size_t (index)].name;
    return TRANS ("unknown");
}
Colour EqProcessor::getBandColour (size_t index) const
{
    if (isPositiveAndBelow (index, bands.size()))
        return bands [size_t (index)].colour;
    return Colours::silver;
}

bool EqProcessor::getBandSolo (int index) const
{
    return index == soloed;
}

void EqProcessor::setBandSolo (int index)
{
    soloed = index;
    updateBypassedStates();
}

void EqProcessor::updateBypassedStates ()
{
    if (isPositiveAndBelow (soloed, bands.size())) {
        filter.setBypassed<0>(soloed != 0);
        filter.setBypassed<1>(soloed != 1);
        filter.setBypassed<2>(soloed != 2);
        filter.setBypassed<3>(soloed != 3);
        filter.setBypassed<4>(soloed != 4);
        filter.setBypassed<5>(soloed != 5);
    }
    else {
        filter.setBypassed<0>(!bands[0].active);
        filter.setBypassed<1>(!bands[1].active);
        filter.setBypassed<2>(!bands[2].active);
        filter.setBypassed<3>(!bands[3].active);
        filter.setBypassed<4>(!bands[4].active);
        filter.setBypassed<5>(!bands[5].active);
    }
}

EqProcessor::Band* EqProcessor::getBand (size_t index)
{
    if (isPositiveAndBelow (index, bands.size()))
        return &bands [index];
    return nullptr;
}

StringArray EqProcessor::getFilterTypeNames()
{
    return {
        TRANS ("No Filter"),
        TRANS ("High Pass"),
        TRANS ("1st High Pass"),
        TRANS ("Low Shelf"),
        TRANS ("Band Pass"),
        TRANS ("All Pass"),
        TRANS ("1st All Pass"),
        TRANS ("Notch"),
        TRANS ("Peak"),
        TRANS ("High Shelf"),
        TRANS ("1st Low Pass"),
        TRANS ("Low Pass")
    };
}

void EqProcessor::updateBand (const size_t index)
{
    if (sampleRate > 0) {
        dsp::IIR::Coefficients<float>::Ptr newCoefficients;
        switch (bands [index].type) {
            case NoFilter:
                newCoefficients = new dsp::IIR::Coefficients<float> (1, 0, 1, 0);
                break;
            case LowPass:
                newCoefficients = dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, bands [index].frequency, bands [index].quality);
                break;
            case LowPass1st:
                newCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderLowPass (sampleRate, bands [index].frequency);
                break;
            case LowShelf:
                newCoefficients = dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, bands [index].frequency, bands [index].quality, bands [index].gain);
                break;
            case BandPass:
                newCoefficients = dsp::IIR::Coefficients<float>::makeBandPass (sampleRate, bands [index].frequency, bands [index].quality);
                break;
            case AllPass:
                newCoefficients = dsp::IIR::Coefficients<float>::makeAllPass (sampleRate, bands [index].frequency, bands [index].quality);
                break;
            case AllPass1st:
                newCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderAllPass (sampleRate, bands [index].frequency);
                break;
            case Notch:
                newCoefficients = dsp::IIR::Coefficients<float>::makeNotch (sampleRate, bands [index].frequency, bands [index].quality);
                break;
            case Peak:
                newCoefficients = dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, bands [index].frequency, bands [index].quality, bands [index].gain);
                break;
            case HighShelf:
                newCoefficients = dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, bands [index].frequency, bands [index].quality, bands [index].gain);
                break;
            case HighPass1st:
                newCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderHighPass (sampleRate, bands [index].frequency);
                break;
            case HighPass:
                newCoefficients = dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, bands [index].frequency, bands [index].quality);
                break;
            default:
                break;
        }

        if (newCoefficients)
        {
            {
                // minimise lock scope, get<0>() needs to be a  compile time constant
                ScopedLock processLock (getCallbackLock());
                if (index == 0)
                    *filter.get<0>().state = *newCoefficients;
                else if (index == 1)
                    *filter.get<1>().state = *newCoefficients;
                else if (index == 2)
                    *filter.get<2>().state = *newCoefficients;
                else if (index == 3)
                    *filter.get<3>().state = *newCoefficients;
                else if (index == 4)
                    *filter.get<4>().state = *newCoefficients;
                else if (index == 5)
                    *filter.get<5>().state = *newCoefficients;
            }
            newCoefficients->getMagnitudeForFrequencyArray (frequencies.data(),
                                                            bands [index].magnitudes.data(),
                                                            frequencies.size(), sampleRate);

        }
        updateBypassedStates();
    }
}

//==============================================================================
bool EqProcessor::hasEditor() const
{
    return true;
}

const std::vector<double>& EqProcessor::getMagnitudes ()
{
    return magnitudes;
}

bool EqProcessor::checkForNewAnalyserData()
{
    return inputAnalyser.checkForNewData() || outputAnalyser.checkForNewData();
}
