/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveshaperProcessor.h"
#define GAIN_ID "gain"
#define GAIN_NAME "Gain"
#define OUTPUT_ID "output"
#define OUTPUT_NAME "Output"
#define LOW_SHELF_GAIN_ID "low_shelf_gain"
#define LOW_SHELF_GAIN_NAME "LS Gain"
#define LOW_SHELF_FREQ_ID "low_shelf_freq"
#define LOW_SHELF_FREQ_NAME "LS Freq"
#define WAVESHAPER_TYPE_ID "waveshaper_type"
#define WAVESHAPER_TYPE_NAME "WS Type"

//==============================================================================
/**
*/
class ShittyAmpAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ShittyAmpAudioProcessor();
    ~ShittyAmpAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void updateWaveshaperParams();
    
    WaveshaperType waveshaperType;
    float gain { 0.5f };
    float outLevel { 0.5f };
    float lowShelfGain { 0.f };
    float lowShelfFreq { 300.f };
    
    AudioProcessorValueTreeState treeState;
private:
    WaveshaperProcessor waveshaperProcessor;
    float lastSampleRate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShittyAmpAudioProcessor)
};
