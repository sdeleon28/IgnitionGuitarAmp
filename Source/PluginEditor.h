#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "DialLookAndFeel.h"
#include "StyledComponent.h"
#include "MainFont.h"

using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

class ShittyAmpAudioProcessorEditor
    : public AudioProcessorEditor
{
public:
    ShittyAmpAudioProcessorEditor(ShittyAmpAudioProcessor&);
    ~ShittyAmpAudioProcessorEditor() override;

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

private:
    std::shared_ptr<Label> gainLabel;
    std::shared_ptr<Slider> gainSlider;
    std::unique_ptr<SliderAttachment> gainValue;

    std::shared_ptr<Label> toneLabel;
    std::shared_ptr<Slider> toneSlider;
    std::unique_ptr<SliderAttachment> toneValue;

    std::shared_ptr<Label> outLevelLabel;
    std::shared_ptr<Slider> outLevelSlider;
    std::unique_ptr<SliderAttachment> outLevelValue;

    std::shared_ptr<Label> thmLabel;
    std::shared_ptr<Label> ignitionLabel;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ShittyAmpAudioProcessor& audioProcessor;

    DialLookAndFeel dialLookAndFeel;
    StyledComponent topComponent;

    MainFont mainFont;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShittyAmpAudioProcessorEditor)
};
