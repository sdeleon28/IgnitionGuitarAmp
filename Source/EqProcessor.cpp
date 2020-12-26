#include "EqProcessor.h"

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

std::vector<EqProcessor::Band> createDefaultBands()
{
    std::vector<EqProcessor::Band> defaults;
    defaults.push_back (EqProcessor::Band (TRANS ("Lowest"),    EqProcessor::HighPass,    20.0f, 0.707f));
    defaults.push_back (EqProcessor::Band (TRANS ("Low"),       EqProcessor::LowShelf,   250.0f, 0.707f));
    defaults.push_back (EqProcessor::Band (TRANS ("Low Mids"),  EqProcessor::Peak,       500.0f, 0.707f));
    defaults.push_back (EqProcessor::Band (TRANS ("High Mids"), EqProcessor::Peak,      1000.0f, 0.707f));
    defaults.push_back (EqProcessor::Band (TRANS ("High"),      EqProcessor::HighShelf, 5000.0f, 0.707f));
    defaults.push_back (EqProcessor::Band (TRANS ("Highest"),   EqProcessor::LowPass,  12000.0f, 0.707f));
    return defaults;
}

//==============================================================================
EqProcessor::EqProcessor()
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
    }
}

EqProcessor::~EqProcessor()
{
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

void EqProcessor::reset () noexcept
{
}

//==============================================================================
void EqProcessor::prepare (const dsp::ProcessSpec& spec) noexcept
{
    sampleRate = spec.sampleRate;

    for (size_t i=0; i < bands.size(); ++i) {
        updateBand (i);
    }
    // Don't push the level by default
    filter.get<6>().setGainLinear (1);

    filter.prepare (spec);
}

void EqProcessor::releaseResources()
{
}

void EqProcessor::process (const dsp::ProcessContextReplacing<float>& context) noexcept
{
    filter.process (context);
}

// BEGIN: Adapter code to make this usable without a UI
void EqProcessor::setGain(float newValueInDb)
{
    filter.get<6>().setGainDecibels(newValueInDb);
}

void EqProcessor::setBand(int index, const EqProcessor::Band& newBand)
{
    if (isPositiveAndBelow (index, bands.size()))
    {
        auto* band = getBand (size_t (index));
        band->type = newBand.type;
        band->frequency = newBand.frequency;
        band->quality = newBand.quality;
        band->gain = newBand.gain;
        band->active = newBand.active;
        updateBand (size_t (index));
    }
}
// END: Adapter code

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
                // I think I don't need the next lock since I'm not changing these values in real time
                // Commenting out for now

                // minimise lock scope, get<0>() needs to be a  compile time constant
                // ScopedLock processLock (getCallbackLock());
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

const std::vector<double>& EqProcessor::getMagnitudes ()
{
    return magnitudes;
}
