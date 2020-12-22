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
           std::make_unique<AudioParameterFloat> (GAIN_ID, GAIN_NAME, 0.f, 10.f, 1.f),
           std::make_unique<AudioParameterFloat> (OUTPUT_ID, OUTPUT_NAME, 0.f, 10.f, 10.f),
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
        EqProcessor::FilterType::HighPass,
        64.0f, // frequency
        0.1f // q
    );
    EqProcessor::Band preEqLow(
        "Low",
        EqProcessor::FilterType::NoFilter,
        0.0f, // frequency
        0.0f, // q
        0.0f, // gain
        false // active
    );
    EqProcessor::Band preEqLowMids(
        "Low Mids",
        EqProcessor::FilterType::Peak,
        671.0f, // frequency
        2.1f, // q
        Decibels::decibelsToGain(8.5f) // gain
    );
    EqProcessor::Band preEqHighMids(
        "High Mids",
        EqProcessor::FilterType::Peak,
        1320.0f, // frequency
        1.1f, // q
        Decibels::decibelsToGain(8.9f) // gain
    );
    EqProcessor::Band preEqHigh(
        "High",
        EqProcessor::FilterType::NoFilter,
        0.0f, // frequency
        0.0f, // q
        0.0f, // gain
        false // active
    );
    EqProcessor::Band preEqHighest(
        "Highest",
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

    gainProcessor.reset();
    gainProcessor.prepare(spec);

    waveshaperProcessor.reset();
    updateWaveshaperParams();
    waveshaperProcessor.prepare(spec);

    // BEGIN post-EQ
    EqProcessor::Band postEqLowest(
        "Lowest",
        EqProcessor::FilterType::HighPass,
        144.0f, // frequency
        1.1f // q
    );
    EqProcessor::Band postEqLow(
        "Low",
        EqProcessor::FilterType::NoFilter,
        0.0f, // frequency
        0.0f, // q
        0.0f, // gain
        false // active
    );
    EqProcessor::Band postEqLowMids(
        "Low Mids",
        EqProcessor::FilterType::Peak,
        304.0f, // frequency
        1.1f, // q
        Decibels::decibelsToGain(2.0f) // gain
    );
    EqProcessor::Band postEqHighMids(
        "High Mids",
        EqProcessor::FilterType::Peak,
        896.0f, // frequency
        1.1f, // q
        Decibels::decibelsToGain(3.6f) // gain
    );
    EqProcessor::Band postEqHigh(
        "High",
        EqProcessor::FilterType::Peak,
        2680.0f, // frequency
        1.1f, // q
        Decibels::decibelsToGain(-0.8f) // gain
    );
    EqProcessor::Band postEqHighest(
        "Highest",
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
    postEqProcessor.setGain(6);

    postEqProcessor.prepare(spec);
    // END post-EQ

    cabConvolutionProcessor.reset();
    cabConvolutionProcessor.prepare(spec);

    outputLevelProcessor.reset();
    outputLevelProcessor.prepare(spec);
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
    // The gain knob works as an amplifier. I empirically found +40dB to be a reasonable
    // upper bound for the amount of amplification allowed.
    // The user facing values are in the familiar 0-10 gain range. Under the hood, this is
    // translated linearly to 0-40dB (by just multiplying by 4). Then that is mapped logarithmically
    // to absolute gain values by the gainProcessor.
    gainProcessor.setGainDecibels(gain * 4);
    // The out level knob works as an attenuator. You use this to tame the amount of gain
    // you've added before the waveshaper (but there's no reason to keep boosting at this stage).
    // The 0-10 range in the output knob should map to -inf to 0 in the dB realm.
    // If you define -inf as -60.f what we need to use to translate user-values into dB is the linear
    // function f(x) such that,
    // f(0) = -60 and f(10) = 0
    // Some basic math reveals that that function is:
    float outAttenuationInDb = outLevelACoefficient * outLevel + outLowerBoundInDb;
    outputLevelProcessor.setGainLinear(Decibels::decibelsToGain(outAttenuationInDb, outLowerBoundInDb));
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
    gainProcessor.process(context);
    waveshaperProcessor.process(context);
    postEqProcessor.process(context);
    cabConvolutionProcessor.process(context);
    outputLevelProcessor.process(context);
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
    std::unique_ptr<XmlElement> xml (treeState.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ShittyAmpAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr <XmlElement> theParams(getXmlFromBinary(data, sizeInBytes));
    if (theParams != nullptr)
    {
        if (theParams -> hasTagName(treeState.state.getType()))
        {
            treeState.state = ValueTree::fromXml(*theParams);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ShittyAmpAudioProcessor();
}
