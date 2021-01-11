#pragma once

#include <JuceHeader.h>
#include "EqProcessor.h"
#include "SingleEqBandProcessor.h"
#include "AsymptoticLimitWaveshaperProcessor.h"
#include "EnvelopeFollower.h"
#include "DynamicWaveshaper.h"
#include "CabConvolutionProcessor.h"

#define GAIN_ID "gain"
#define GAIN_NAME "Gain"
#define OUTPUT_ID "output"
#define OUTPUT_NAME "Output"
#define TONE_ID "tone"
#define TONE_NAME "Tone"

//==============================================================================
/**
 */
class ShittyAmpAudioProcessor : public juce::AudioProcessor
{
  public:
    //==============================================================================
    ShittyAmpAudioProcessor();
    ~ShittyAmpAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    void updateParams();

    float gain;
    float outLevel;

    AudioProcessorValueTreeState treeState;

  private:
    EqProcessor preEqProcessor;
    Gain<float> gainProcessor;
    Gain<float> middleGainProcessor;
    AsymptoticLimitWaveshaperProcessor asymptoticLimitWaveshaper;
    EnvelopeFollower envelopeFollower;
    DynamicWaveshaper dynamicWaveshaper;
    EqProcessor postEqProcessor;
    SingleEqBandProcessor toneControlEqProcessor;
    CabConvolutionProcessor cabConvolutionProcessor;
    Gain<float> outputLevelProcessor;
    SingleEqBandProcessor postConvolutionLowEqProcessor;
    SingleEqBandProcessor postConvolutionLowMidsEqProcessor;

    float outLowerBoundInDb = -60.0;
    float outLevelACoefficient = -outLowerBoundInDb / 10;

    float lastSampleRate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShittyAmpAudioProcessor)
};
