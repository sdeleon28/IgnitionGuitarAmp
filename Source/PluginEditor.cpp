/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

//==============================================================================
ShittyAmpAudioProcessorEditor::ShittyAmpAudioProcessorEditor(ShittyAmpAudioProcessor &p)
    : AudioProcessorEditor(&p)
    , audioProcessor(p)
{
    gainLabel.setText(GAIN_NAME, dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);
    gainValue = std::make_unique<SliderAttachment>(audioProcessor.treeState, GAIN_ID, gainSlider);
    gainSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    gainSlider.setRange(0.0f, 10.0f, 0.1f);
    gainSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(gainSlider);

    toneLabel.setText(TONE_NAME, dontSendNotification);
    toneLabel.attachToComponent(&toneSlider, false);
    toneValue = std::make_unique<SliderAttachment>(audioProcessor.treeState, TONE_ID, toneSlider);
    toneSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    toneSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    toneSlider.setRange(0.0f, 10.0f, 0.1f);
    toneSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(toneLabel);
    addAndMakeVisible(toneSlider);

    outLevelLabel.setText(OUTPUT_NAME, dontSendNotification);
    outLevelLabel.attachToComponent(&outLevelSlider, false);
    outLevelValue = std::make_unique<SliderAttachment>(audioProcessor.treeState, OUTPUT_ID, outLevelSlider);
    outLevelSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    outLevelSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    outLevelSlider.setRange(0.f, 10.0f, 0.1f);
    outLevelSlider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(outLevelLabel);
    addAndMakeVisible(outLevelSlider);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 600);
}

ShittyAmpAudioProcessorEditor::~ShittyAmpAudioProcessorEditor()
{
}

//==============================================================================
void ShittyAmpAudioProcessorEditor::paint(Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    g.setColour(Colours::white);
    g.setFont(15.0f);
}

void ShittyAmpAudioProcessorEditor::placeKnob(Slider *slider, int column, int row)
{
    const int rowSize = 100;
    const int columnSize = 100;
    slider->setBounds(column * columnSize, row * rowSize, columnSize * 0.8, rowSize * 0.8);
}

void ShittyAmpAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // First row
    placeKnob(&gainSlider, 0, 1);
    placeKnob(&toneSlider, 1, 1);
    placeKnob(&outLevelSlider, 2, 1);
}
