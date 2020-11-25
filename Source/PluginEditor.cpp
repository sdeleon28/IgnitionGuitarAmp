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
    mWaveshaperTypeComboBox.addItem("Hyperbolic tangent", ShittyAmpAudioProcessor::WaveshaperType::hyperbolicTangent);
    mWaveshaperTypeComboBox.addItem("Square", ShittyAmpAudioProcessor::WaveshaperType::square);
    mWaveshaperTypeComboBox.addItem("Sinewave", ShittyAmpAudioProcessor::WaveshaperType::sinewave);
    mWaveshaperTypeComboBox.addListener(this);
    addAndMakeVisible(mWaveshaperTypeComboBox);
    
    mGainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    mGainSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    mGainSlider.setRange(0.0f, 20.0f, 0.01f);
    mGainSlider.setValue(1.f);
    mGainSlider.setPopupDisplayEnabled(true, true, this);
    mGainSlider.addListener(this);
    addAndMakeVisible(mGainSlider);

    mOutLevelSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    mOutLevelSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    mOutLevelSlider.setRange(0.0f, 1.0f, 0.01f);
    mOutLevelSlider.setValue(0.5f);
    mOutLevelSlider.setPopupDisplayEnabled(true, true, this);
    mOutLevelSlider.addListener(this);
    addAndMakeVisible(mOutLevelSlider);

    mLowShelfGainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    mLowShelfGainSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    mLowShelfGainSlider.setRange(-10.f, 10.f, 0.01f);
    mLowShelfGainSlider.setValue(0.5f);
    mLowShelfGainSlider.setPopupDisplayEnabled(true, true, this);
    mLowShelfGainSlider.addListener(this);
    addAndMakeVisible(mLowShelfGainSlider);

    mLowShelfFreqSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    mLowShelfFreqSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    mLowShelfFreqSlider.setRange(20, 600, 0.01f);
    mLowShelfFreqSlider.setValue(0.5f);
    mLowShelfFreqSlider.setPopupDisplayEnabled(true, true, this);
    mLowShelfFreqSlider.addListener(this);
    addAndMakeVisible(mLowShelfFreqSlider);

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
    mWaveshaperTypeComboBox.setBounds(0, 0, 200, 50);
    mGainSlider.setBounds(0, 1 * row, 100, 100);
    mOutLevelSlider.setBounds(1 * column, 1 * row, 100, 100);
    mLowShelfGainSlider.setBounds(0, 2 * row, 100, 100);
    mLowShelfFreqSlider.setBounds(1 * column, 2 * row, 100, 100);
}

void ShittyAmpAudioProcessorEditor::comboBoxChanged(ComboBox *comboBox)
{
    if (comboBox == &mWaveshaperTypeComboBox)
    {
        const int selectedId = mWaveshaperTypeComboBox.getSelectedId();
        if (selectedId == ShittyAmpAudioProcessor::WaveshaperType::hyperbolicTangent)
        {
            audioProcessor.mWaveshaperType = ShittyAmpAudioProcessor::WaveshaperType::hyperbolicTangent;
        }
        else if (selectedId == ShittyAmpAudioProcessor::WaveshaperType::square)
        {
            audioProcessor.mWaveshaperType = ShittyAmpAudioProcessor::WaveshaperType::square;
        }
        else if (selectedId == ShittyAmpAudioProcessor::WaveshaperType::sinewave)
        {
            audioProcessor.mWaveshaperType = ShittyAmpAudioProcessor::WaveshaperType::sinewave;
        }
    }
}

void ShittyAmpAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if (slider == &mGainSlider)
        audioProcessor.mGain = mGainSlider.getValue();
    if (slider == &mOutLevelSlider)
        audioProcessor.mOutLevel = mOutLevelSlider.getValue();
}
