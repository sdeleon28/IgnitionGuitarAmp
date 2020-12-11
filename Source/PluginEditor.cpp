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
    waveshaperTypeComboBox.addItem("Asymptotic limit", WaveshaperType::asymptoticLimit);
    waveshaperTypeComboBox.addItem("Hyperbolic tangent", WaveshaperType::hyperbolicTangent);
    waveshaperTypeComboBox.addItem("Square", WaveshaperType::square);
    waveshaperTypeComboBox.addItem("Sinewave", WaveshaperType::sinewave);
    waveshaperTypeValue = std::make_unique<ComboBoxAttachment>(audioProcessor.treeState, WAVESHAPER_TYPE_ID, waveshaperTypeComboBox);
    waveshaperTypeComboBox.addListener(this);
    addAndMakeVisible(waveshaperTypeComboBox);
    
    gainLabel.setText(GAIN_NAME, dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);
    gainValue = std::make_unique<SliderAttachment>(audioProcessor.treeState, GAIN_ID, gainSlider);
    gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    gainSlider.setRange(0.0f, 10.0f, 0.01f);
    gainSlider.setValue(82.f);
    gainSlider.setPopupDisplayEnabled(true, true, this);
    gainSlider.addListener(this);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(gainSlider);

    outLevelLabel.setText(OUTPUT_NAME, dontSendNotification);
    outLevelLabel.attachToComponent(&outLevelSlider, false);
    outLevelValue = std::make_unique<SliderAttachment>(audioProcessor.treeState, OUTPUT_ID, outLevelSlider);
    outLevelSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    outLevelSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    outLevelSlider.setRange(0.0f, 1.0f, 0.01f);
    outLevelSlider.setValue(0.15f);
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
    waveshaperTypeComboBox.setBounds(0, 0, 200, 50);
    gainSlider.setBounds(0, 1 * row, column * 0.8, row * 0.8);
    outLevelSlider.setBounds(1 * column, 1 * row, column * 0.8, row * 0.8);
}

void ShittyAmpAudioProcessorEditor::comboBoxChanged(ComboBox *comboBox)
{
    if (comboBox == &waveshaperTypeComboBox)
    {
        const int selectedId = waveshaperTypeComboBox.getSelectedId();
        if (selectedId == WaveshaperType::asymptoticLimit)
        {
            audioProcessor.waveshaperType = WaveshaperType::asymptoticLimit;
        }
        else if (selectedId == WaveshaperType::hyperbolicTangent)
        {
            audioProcessor.waveshaperType = WaveshaperType::hyperbolicTangent;
        }
        else if (selectedId == WaveshaperType::square)
        {
            audioProcessor.waveshaperType = WaveshaperType::square;
        }
        else if (selectedId == WaveshaperType::sinewave)
        {
            audioProcessor.waveshaperType = WaveshaperType::sinewave;
        }
    }
}

void ShittyAmpAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if (slider == &gainSlider)
        audioProcessor.gain = gainSlider.getValue();
    if (slider == &outLevelSlider)
        audioProcessor.outLevel = outLevelSlider.getValue();
}
