#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "DialLookAndFeel.h"

#define SIZE_FACTOR 1
#define SEPARATOR_PINK 20
#define SEPARATOR_GREEN 40
#define SEPARATOR_BLUE 88
#define COLOUR_BLACK Colour(28, 28, 28)
#define COLOUR_YELLOW Colour(249, 226, 85)
#define COLOUR_LIGHT_GREY Colour(142, 142, 142)
#define COLOUR_DARK_GREY Colour(58, 58, 58)


using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

struct Box {
    float top;
    float left;
    float height;
    float width;
    float cornerSize = 0;
    float borderThickness = 0;
    Colour colour;
    Colour borderColour = COLOUR_BLACK;

    float getBottom()
    {
        return top + height;
    }
    
    float getRight()
    {
        return left + width;
    }
};


class ShittyAmpAudioProcessorEditor
    : public AudioProcessorEditor
{
  public:
    ShittyAmpAudioProcessorEditor(ShittyAmpAudioProcessor &);
    ~ShittyAmpAudioProcessorEditor() override;

    //==============================================================================
    void paint(Graphics &) override;
    void placeKnob(Slider *slider, int column, int row);
    void resized() override;

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
    ShittyAmpAudioProcessor &audioProcessor;

    DialLookAndFeel dialLookAndFeel;

    Box topYellowBox;
    Box midYellowBox;
    Box topGreyBox;
    Box topBlackBox;
    Box bottomYellowBox;
    Box bottomGreyBox;
    Box bottomBlackBox;
    Box gainBox;
    Box toneBox;
    Box levelBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShittyAmpAudioProcessorEditor)
};
