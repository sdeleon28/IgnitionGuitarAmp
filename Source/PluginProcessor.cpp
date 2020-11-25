/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

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
           std::make_unique<AudioParameterFloat> (GAIN_ID, GAIN_NAME, 1.f, 20.f, 1.f),
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    //lastSampleRate = sampleRate;

    //dsp::ProcessSpec spec;
    //spec.sampleRate = lastSampleRate;
    //spec.maximumBlockSize = samplesPerBlock;
    //spec.numChannels = getMainBusNumOutputChannels();

    //stateVariableFilter.reset();
    //stateVariableFilter.prepare(spec);
}

void ShittyAmpAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float processedSample = channelData[sample]; // Default to same sample
            if (waveshaperType == WaveshaperType::hyperbolicTangent)
            {
                processedSample = std::tanh(channelData[sample] * gain);
            }
            else if (waveshaperType == WaveshaperType::square)
            {
                processedSample = (channelData[sample] >= 0 ? 1.f : -1.f);
            }
            else if (waveshaperType == WaveshaperType::sinewave)
            {
                processedSample = std::sin(channelData[sample] * gain);
            }
            channelData[sample] = jlimit(-1.f, 1.f, processedSample) * outLevel;
        }
    }
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
