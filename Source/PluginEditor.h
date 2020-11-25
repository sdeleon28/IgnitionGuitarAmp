/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

//==============================================================================
/**
*/
class ShittyAmpAudioProcessorEditor  : public AudioProcessorEditor,
                                       public Slider::Listener,
                                       public ComboBox::Listener
{
public:
    ShittyAmpAudioProcessorEditor (ShittyAmpAudioProcessor&);
    ~ShittyAmpAudioProcessorEditor() override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider* slider) override;
    void comboBoxChanged (ComboBox* comboBox) override;
private:
    Label mGainLabel;
    Slider mGainSlider;
    std::unique_ptr<SliderAttachment> mGainAttachment;

    Label mOutLevelLabel;
    Slider mOutLevelSlider;
    std::unique_ptr<SliderAttachment> mOutLevelAttacment;

    Label mLowShelfGainLabel;
    Slider mLowShelfGainSlider;
    std::unique_ptr<SliderAttachment> mLowShelfGainAttacment;

    Label mLowShelfFreqLabel;
    Slider mLowShelfFreqSlider;
    std::unique_ptr<SliderAttachment> mLowShelfFreqAttacment;

    Label mWaveshaperTypeLabel;
    ComboBox mWaveshaperTypeComboBox;
    std::unique_ptr<SliderAttachment> mWaveshaperTypeAttacment;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ShittyAmpAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShittyAmpAudioProcessorEditor)
};
