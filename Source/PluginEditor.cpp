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
    mGainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    mGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    mGainSlider.setRange(0.0f, 20.0f, 0.01f);
    mGainSlider.setValue(1.f);
    mGainSlider.addListener(this);

    mOutLevelSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    mOutLevelSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    mOutLevelSlider.setRange(0.0f, 1.0f, 0.01f);
    mOutLevelSlider.setValue(0.5f);
    mOutLevelSlider.addListener(this);

    mWaveshaperTypeComboBox.addItem("Hyperbolic tangent", ShittyAmpAudioProcessor::WaveshaperType::hyperbolicTangent);
    mWaveshaperTypeComboBox.addItem("Square", ShittyAmpAudioProcessor::WaveshaperType::square);
    mWaveshaperTypeComboBox.addListener(this);

    addAndMakeVisible(mWaveshaperTypeComboBox);
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
void ShittyAmpAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void ShittyAmpAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mWaveshaperTypeComboBox.setBounds(0, 0, 100, 50);
    mGainSlider.setBounds(100, getHeight() / 2 - 75, 100, 150);
    mOutLevelSlider.setBounds(300, getHeight() / 2 - 75, 100, 150);
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
    }
}

void ShittyAmpAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if (slider == &mGainSlider)
        audioProcessor.mGain = mGainSlider.getValue();
    if (slider == &mOutLevelSlider)
        audioProcessor.mOutLevel = mOutLevelSlider.getValue();
}
