/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;

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
    Label gainLabel;
    Slider gainSlider;
    std::unique_ptr<SliderAttachment> gainValue;

    Label outLevelLabel;
    Slider outLevelSlider;
    std::unique_ptr<SliderAttachment> outLevelValue;

    Label waveshaperTypeLabel;
    ComboBox waveshaperTypeComboBox;
    std::unique_ptr<ComboBoxAttachment> waveshaperTypeValue;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ShittyAmpAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShittyAmpAudioProcessorEditor)
};
