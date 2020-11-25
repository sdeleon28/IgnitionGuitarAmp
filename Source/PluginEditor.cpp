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
    // TODO: Set label
    // TODO: Connect to parameter
    waveshaperTypeComboBox.addItem("Hyperbolic tangent", ShittyAmpAudioProcessor::WaveshaperType::hyperbolicTangent);
    waveshaperTypeComboBox.addItem("Square", ShittyAmpAudioProcessor::WaveshaperType::square);
    waveshaperTypeComboBox.addItem("Sinewave", ShittyAmpAudioProcessor::WaveshaperType::sinewave);
    waveshaperTypeComboBox.addListener(this);
    addAndMakeVisible(waveshaperTypeComboBox);
    
    gainLabel.setText(GAIN_NAME, dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);
    gainAttachment = std::make_unique<SliderAttachment>(audioProcessor.treeState, GAIN_ID, gainSlider);
    gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    gainSlider.setRange(0.0f, 20.0f, 0.01f);
    gainSlider.setValue(1.f);
    gainSlider.setPopupDisplayEnabled(true, true, this);
    gainSlider.addListener(this);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(gainSlider);

    // TODO: Set label
    // TODO: Connect to parameter
    outLevelSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    outLevelSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    outLevelSlider.setRange(0.0f, 1.0f, 0.01f);
    outLevelSlider.setValue(0.5f);
    outLevelSlider.setPopupDisplayEnabled(true, true, this);
    outLevelSlider.addListener(this);
    addAndMakeVisible(outLevelSlider);

    // TODO: Set label
    // TODO: Connect to parameter
    lowShelfGainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    lowShelfGainSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    lowShelfGainSlider.setRange(-10.f, 10.f, 0.01f);
    lowShelfGainSlider.setValue(0.5f);
    lowShelfGainSlider.setPopupDisplayEnabled(true, true, this);
    lowShelfGainSlider.addListener(this);
    addAndMakeVisible(lowShelfGainSlider);

    // TODO: Set label
    // TODO: Connect to parameter
    lowShelfFreqSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    lowShelfFreqSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    lowShelfFreqSlider.setRange(20, 600, 0.01f);
    lowShelfFreqSlider.setValue(0.5f);
    lowShelfFreqSlider.setPopupDisplayEnabled(true, true, this);
    lowShelfFreqSlider.addListener(this);
    addAndMakeVisible(lowShelfFreqSlider);

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
    waveshaperTypeComboBox.setBounds(0, 0, 200, 50);
    gainSlider.setBounds(0, 1 * row, 100, 100);
    outLevelSlider.setBounds(1 * column, 1 * row, 100, 100);
    lowShelfGainSlider.setBounds(0, 2 * row, 100, 100);
    lowShelfFreqSlider.setBounds(1 * column, 2 * row, 100, 100);
}

void ShittyAmpAudioProcessorEditor::comboBoxChanged(ComboBox *comboBox)
{
    if (comboBox == &waveshaperTypeComboBox)
    {
        const int selectedId = waveshaperTypeComboBox.getSelectedId();
        if (selectedId == ShittyAmpAudioProcessor::WaveshaperType::hyperbolicTangent)
        {
            audioProcessor.waveshaperType = ShittyAmpAudioProcessor::WaveshaperType::hyperbolicTangent;
        }
        else if (selectedId == ShittyAmpAudioProcessor::WaveshaperType::square)
        {
            audioProcessor.waveshaperType = ShittyAmpAudioProcessor::WaveshaperType::square;
        }
        else if (selectedId == ShittyAmpAudioProcessor::WaveshaperType::sinewave)
        {
            audioProcessor.waveshaperType = ShittyAmpAudioProcessor::WaveshaperType::sinewave;
        }
    }
}

void ShittyAmpAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if (slider == &gainSlider)
        audioProcessor.gain = gainSlider.getValue();
    if (slider == &outLevelSlider)
        audioProcessor.outLevel = outLevelSlider.getValue();
    // TODO: add filters
}
