#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ShittyAmpAudioProcessor::ShittyAmpAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       treeState(*this, nullptr, "PARAMETERS", {
           std::make_unique<AudioParameterFloat> (WAVESHAPER_TYPE_ID, WAVESHAPER_TYPE_NAME, 1.f, 4.f, 1.f),
           std::make_unique<AudioParameterFloat> (GAIN_ID, GAIN_NAME, 1.f, 100.f, 1.f),
           std::make_unique<AudioParameterFloat> (OUTPUT_ID, OUTPUT_NAME, 0.f, 1.f, 0.5f),
           std::make_unique<AudioParameterFloat> (LOW_SHELF_GAIN_ID, LOW_SHELF_GAIN_NAME, -10.f, 10.f, 0.f),
           std::make_unique<AudioParameterFloat> (LOW_SHELF_FREQ_ID, LOW_SHELF_FREQ_NAME, 20.f, 1000.f, 490.f),
       })
#endif
{
}

ShittyAmpAudioProcessor::~ShittyAmpAudioProcessor()
{
}

//==============================================================================
const juce::String ShittyAmpAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ShittyAmpAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ShittyAmpAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ShittyAmpAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ShittyAmpAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ShittyAmpAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ShittyAmpAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ShittyAmpAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ShittyAmpAudioProcessor::getProgramName (int index)
{
    return {};
}

void ShittyAmpAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ShittyAmpAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastSampleRate = sampleRate;

    dsp::ProcessSpec spec;
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();

    // BEGIN pre-EQ
    EqProcessor::Band preEqLowest(
        "Lowest",
        Colours::blue, // TODO: Get rid of this
        EqProcessor::FilterType::HighPass,
        64.0f, // frequency
        0.1f // q
    );
    EqProcessor::Band preEqLow(
        "Low",
        Colours::blue, // TODO: Get rid of this
        EqProcessor::FilterType::NoFilter,
        0.0f, // frequency
        0.0f, // q
        0.0f, // gain
        false // active
    );
    EqProcessor::Band preEqLowMids(
        "Low Mids",
        Colours::blue, // TODO: Get rid of this
        EqProcessor::FilterType::Peak,
        671.0f, // frequency
        2.1f, // q
        Decibels::decibelsToGain(8.5f) // gain
    );
    EqProcessor::Band preEqHighMids(
        "High Mids",
        Colours::blue, // TODO: Get rid of this
        EqProcessor::FilterType::Peak,
        1320.0f, // frequency
        1.1f, // q
        Decibels::decibelsToGain(8.9f) // gain
    );
    EqProcessor::Band preEqHigh(
        "High",
        Colours::blue, // TODO: Get rid of this
        EqProcessor::FilterType::NoFilter,
        0.0f, // frequency
        0.0f, // q
        0.0f, // gain
        false // active
    );
    EqProcessor::Band preEqHighest(
        "Highest",
        Colours::blue, // TODO: Get rid of this
        EqProcessor::FilterType::LowPass,
        17380.0f, // frequency
        0.1f // q
    );

    preEqProcessor.setBand(0, preEqLowest);
    preEqProcessor.setBand(1, preEqLow);
    preEqProcessor.setBand(2, preEqLowMids);
    preEqProcessor.setBand(3, preEqHighMids);
    preEqProcessor.setBand(4, preEqHigh);
    preEqProcessor.setBand(5, preEqHighest);

    preEqProcessor.prepare(spec);
    // END pre-EQ

    waveshaperProcessor.reset();
    updateWaveshaperParams();
    waveshaperProcessor.prepare(spec);

    // BEGIN post-EQ
    EqProcessor::Band postEqLowest(
        "Lowest",
        Colours::blue, // TODO: Get rid of this
        EqProcessor::FilterType::HighPass,
        144.0f, // frequency
        1.1f // q
    );
    EqProcessor::Band postEqLow(
        "Low",
        Colours::blue, // TODO: Get rid of this
        EqProcessor::FilterType::NoFilter,
        0.0f, // frequency
        0.0f, // q
        0.0f, // gain
        false // active
    );
    EqProcessor::Band postEqLowMids(
        "Low Mids",
        Colours::blue, // TODO: Get rid of this
        EqProcessor::FilterType::Peak,
        304.0f, // frequency
        1.1f, // q
        Decibels::decibelsToGain(2.0f) // gain
    );
    EqProcessor::Band postEqHighMids(
        "High Mids",
        Colours::blue, // TODO: Get rid of this
        EqProcessor::FilterType::Peak,
        896.0f, // frequency
        1.1f, // q
        Decibels::decibelsToGain(3.6f) // gain
    );
    EqProcessor::Band postEqHigh(
        "High",
        Colours::blue, // TODO: Get rid of this
        EqProcessor::FilterType::Peak,
        2680.0f, // frequency
        1.1f, // q
        Decibels::decibelsToGain(-0.8f) // gain
    );
    EqProcessor::Band postEqHighest(
        "Highest",
        Colours::blue, // TODO: Get rid of this
        EqProcessor::FilterType::NoFilter,
        0.0f, // frequency
        0.0f, // q
        0.0f, // gain
        false // active
    );

    postEqProcessor.setBand(0, postEqLowest);
    postEqProcessor.setBand(1, postEqLow);
    postEqProcessor.setBand(2, postEqLowMids);
    postEqProcessor.setBand(3, postEqHighMids);
    postEqProcessor.setBand(4, postEqHigh);
    postEqProcessor.setBand(5, postEqHighest);

    postEqProcessor.prepare(spec);
    // END post-EQ
}

void ShittyAmpAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void ShittyAmpAudioProcessor::updateWaveshaperParams()
{
    gain = *treeState.getRawParameterValue(GAIN_ID);
    outLevel = *treeState.getRawParameterValue(OUTPUT_ID);
    float floatWt = *treeState.getRawParameterValue(WAVESHAPER_TYPE_ID);
    // FIXME: Get rid of this whole waveshaper combo box thing
    /*
    switch ((int)floatWt) {
        case 1:
            waveshaperType = WaveshaperType::asymptoticLimit;
            break;
        case 2:
            waveshaperType = WaveshaperType::hyperbolicTangent;
            break;
        case 3:
            waveshaperType = WaveshaperType::square;
            break;
        case 4:
            waveshaperType = WaveshaperType::sinewave;
            break;
        default:
            break;
    }
    */
    waveshaperType = WaveshaperType::asymptoticLimit;
    waveshaperProcessor.setGain(gain);
    waveshaperProcessor.setOutLevel(outLevel);
    waveshaperProcessor.setWaveshaperType(waveshaperType);
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ShittyAmpAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ShittyAmpAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    dsp::AudioBlock<float> block(buffer);
    updateWaveshaperParams();

    // TODO: Make this a processor chain
    dsp::ProcessContextReplacing<float> context = dsp::ProcessContextReplacing<float>(block);
    preEqProcessor.process(context);
    waveshaperProcessor.process(context);
    postEqProcessor.process(context);
}

//==============================================================================
bool ShittyAmpAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ShittyAmpAudioProcessor::createEditor()
{
    return new ShittyAmpAudioProcessorEditor (*this);
}

//==============================================================================
void ShittyAmpAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ShittyAmpAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ShittyAmpAudioProcessor();
}
