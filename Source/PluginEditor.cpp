/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ShittyAmpAudioProcessorEditor::ShittyAmpAudioProcessorEditor (ShittyAmpAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    mGainSlider.setRange(0.0f, 10.0f, 0.01f);
    mGainSlider.setValue(1.f);
    mGainSlider.addListener(this);

    mOutLevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mOutLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    mOutLevelSlider.setRange(0.0f, 1.0f, 0.01f);
    mOutLevelSlider.setValue(0.5f);
    mOutLevelSlider.addListener(this);

    addAndMakeVisible(mGainSlider);
    addAndMakeVisible(mOutLevelSlider);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
}

ShittyAmpAudioProcessorEditor::~ShittyAmpAudioProcessorEditor()
{
}

//==============================================================================
void ShittyAmpAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void ShittyAmpAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mGainSlider.setBounds(100, getHeight() / 2 - 75, 100, 150);
    mOutLevelSlider.setBounds(300, getHeight() / 2 - 75, 100, 150);
}

void ShittyAmpAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &mGainSlider)
        audioProcessor.mGain = mGainSlider.getValue();
    if (slider == &mOutLevelSlider)
        audioProcessor.mOutLevel = mOutLevelSlider.getValue();
}
