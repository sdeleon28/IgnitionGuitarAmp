/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

//==============================================================================
ShittyAmpAudioProcessorEditor::ShittyAmpAudioProcessorEditor (ShittyAmpAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    gainLabel.setText(GAIN_NAME, dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);
    gainValue = std::make_unique<SliderAttachment>(audioProcessor.treeState, GAIN_ID, gainSlider);
    gainSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    gainSlider.setRange(0.0f, 10.0f, 0.01f);
    gainSlider.setPopupDisplayEnabled(true, true, this);
    gainSlider.addListener(this);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(gainSlider);

    outLevelLabel.setText(OUTPUT_NAME, dontSendNotification);
    outLevelLabel.attachToComponent(&outLevelSlider, false);
    outLevelValue = std::make_unique<SliderAttachment>(audioProcessor.treeState, OUTPUT_ID, outLevelSlider);
    outLevelSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    outLevelSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    outLevelSlider.setRange(0.f, 10.0f, 0.1f);
    outLevelSlider.setPopupDisplayEnabled(true, true, this);
    outLevelSlider.addListener(this);
    addAndMakeVisible(outLevelLabel);
    addAndMakeVisible(outLevelSlider);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
}

ShittyAmpAudioProcessorEditor::~ShittyAmpAudioProcessorEditor()
{
}

//==============================================================================
void ShittyAmpAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void ShittyAmpAudioProcessorEditor::resized()
{
    const int row = 100;
    const int column = 100;
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    gainSlider.setBounds(0 * column, 1 * row, column * 0.8, row * 0.8);
    outLevelSlider.setBounds(1 * column, 1 * row, column * 0.8, row * 0.8);
}

void ShittyAmpAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if (slider == &gainSlider)
        audioProcessor.gain = gainSlider.getValue();
    if (slider == &outLevelSlider)
        audioProcessor.outLevel = outLevelSlider.getValue();
}
