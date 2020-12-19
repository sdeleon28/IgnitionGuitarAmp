/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "EqProcessor.h"
#include "WaveshaperProcessor.h"
#include "CabConvolutionProcessor.h"

#define GAIN_ID "gain"
#define GAIN_NAME "Gain"
#define OUTPUT_ID "output"
#define OUTPUT_NAME "Output"

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
    
    float gain { 5.f };
    float outLevel { 5.f };
    
    AudioProcessorValueTreeState treeState;
private:
    EqProcessor preEqProcessor;
    WaveshaperProcessor waveshaperProcessor;
    EqProcessor postEqProcessor;
    CabConvolutionProcessor cabConvolutionProcessor;
    float outLowerBoundInDb = -60.0;
    float outLevelACoefficient = -outLowerBoundInDb / 10;

    float lastSampleRate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShittyAmpAudioProcessor)
};
