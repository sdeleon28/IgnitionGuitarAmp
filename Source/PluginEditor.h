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
                                       public Slider::Listener
{
public:
    ShittyAmpAudioProcessorEditor (ShittyAmpAudioProcessor&);
    ~ShittyAmpAudioProcessorEditor() override;

    //==============================================================================
    void paint (Graphics&) override;
    void placeKnob(Slider* slider, int column, int row);
    void resized() override;
    void sliderValueChanged (Slider* slider) override;
private:
    Label gainLabel;
    Slider gainSlider;
    std::unique_ptr<SliderAttachment> gainValue;

    Label toneLabel;
    Slider toneSlider;
    std::unique_ptr<SliderAttachment> toneValue;

    Label outLevelLabel;
    Slider outLevelSlider;
    std::unique_ptr<SliderAttachment> outLevelValue;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ShittyAmpAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShittyAmpAudioProcessorEditor)
};
