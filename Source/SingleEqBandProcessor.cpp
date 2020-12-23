
/*
  ==============================================================================

    This is the Frequalizer implementation

  ==============================================================================
*/

#include "SingleEqBandProcessor.h"

String SingleEqBandProcessor::paramOutput   ("output");
String SingleEqBandProcessor::paramType     ("type");
String SingleEqBandProcessor::paramFrequency("frequency");
String SingleEqBandProcessor::paramQuality  ("quality");
String SingleEqBandProcessor::paramGain     ("gain");
String SingleEqBandProcessor::paramActive   ("active");

//==============================================================================
SingleEqBandProcessor::SingleEqBandProcessor()
    : band(SingleEqBandProcessor::Band (TRANS ("High Mids"), SingleEqBandProcessor::Peak, 1000.0f, 0.707f))
{
    frequencies.resize (300);
    for (size_t i=0; i < frequencies.size(); ++i) {
        frequencies [i] = 20.0 * std::pow (2.0, i / 30.0);
    }
    magnitudes.resize (frequencies.size());
    band.magnitudes.resize (frequencies.size(), 1.0);
}

SingleEqBandProcessor::~SingleEqBandProcessor()
{
}

void SingleEqBandProcessor::reset () noexcept
{
}

//==============================================================================
void SingleEqBandProcessor::prepare (const dsp::ProcessSpec& spec) noexcept
{
    sampleRate = spec.sampleRate;
    updateBand();
    filter.prepare (spec);
    filter.reset();
}

void SingleEqBandProcessor::releaseResources()
{
}

void SingleEqBandProcessor::process (const dsp::ProcessContextReplacing<float>& context) noexcept
{
    filter.process (context);
}

void SingleEqBandProcessor::setBand(const SingleEqBandProcessor::Band& newBand)
{
    band.type = newBand.type;
    band.frequency = newBand.frequency;
    band.quality = newBand.quality;
    band.gain = newBand.gain;
    band.active = newBand.active;
    updateBand ();
}

void SingleEqBandProcessor::updateBand ()
{
    if (sampleRate > 0) {
        dsp::IIR::Coefficients<float>::Ptr newCoefficients;
        switch (band.type) {
            case NoFilter:
                newCoefficients = new dsp::IIR::Coefficients<float> (1, 0, 1, 0);
                break;
            case LowPass:
                newCoefficients = dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, band.frequency, band.quality);
                break;
            case LowPass1st:
                newCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderLowPass (sampleRate, band.frequency);
                break;
            case LowShelf:
                newCoefficients = dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, band.frequency, band.quality, band.gain);
                break;
            case BandPass:
                newCoefficients = dsp::IIR::Coefficients<float>::makeBandPass (sampleRate, band.frequency, band.quality);
                break;
            case AllPass:
                newCoefficients = dsp::IIR::Coefficients<float>::makeAllPass (sampleRate, band.frequency, band.quality);
                break;
            case AllPass1st:
                newCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderAllPass (sampleRate, band.frequency);
                break;
            case Notch:
                newCoefficients = dsp::IIR::Coefficients<float>::makeNotch (sampleRate, band.frequency, band.quality);
                break;
            case Peak:
                newCoefficients = dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, band.frequency, band.quality, band.gain);
                break;
            case HighShelf:
                newCoefficients = dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, band.frequency, band.quality, band.gain);
                break;
            case HighPass1st:
                newCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderHighPass (sampleRate, band.frequency);
                break;
            case HighPass:
                newCoefficients = dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, band.frequency, band.quality);
                break;
            default:
                break;
        }

        if (newCoefficients)
        {
            {
                // I think I don't need the next lock since I'm not changing these values in real time
                // Commenting out for now
                // FIXME will probably need the lock again when changing values in real time?
                // See how they do it in https://github.com/TheAudioProgrammer/juceIIRFilter

                // minimise lock scope, get<0>() needs to be a  compile time constant
                // ScopedLock processLock (getCallbackLock());

                *filter.state = *newCoefficients;
            }
            newCoefficients->getMagnitudeForFrequencyArray (frequencies.data(),
                                                            band.magnitudes.data(),
                                                            frequencies.size(), sampleRate);
        }
    }
}

const std::vector<double>& SingleEqBandProcessor::getMagnitudes ()
{
    return magnitudes;
}
