#include <algorithm>
#include <string>
#include "PluginProcessor.h"
#include "PluginEditor.h"

using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

inline std::string upper(std::string str)
{
    std::string out = str;
    std::transform(out.begin(), out.end(), out.begin(), ::toupper);
    return out;
}

//==============================================================================
ShittyAmpAudioProcessorEditor::ShittyAmpAudioProcessorEditor(ShittyAmpAudioProcessor &p)
    : AudioProcessorEditor(&p)
    , audioProcessor(p)
{

    const int borderSize = 3;
    const int separatorPink = SEPARATOR_PINK * SIZE_FACTOR;
    const int separatorBlue = SEPARATOR_BLUE * SIZE_FACTOR;
    const int separatorGreen = SEPARATOR_GREEN * SIZE_FACTOR;
    const int yellowBoxWidth = (504 + 2 * borderSize) * SIZE_FACTOR;

    // TODO:
    // colour -> backgroundColor
    // borderColour -> borderColor
    StyledComponent::Box topYellowBox = {
        .left = separatorPink,
        .top = separatorPink,
        .width = yellowBoxWidth,
        .height = 58 * SIZE_FACTOR,
        .borderRadius = 10,
        .borderWidth = 3,
        .colour = COLOUR_YELLOW,
        .borderColour = COLOUR_BLACK,
    };
    StyledComponent::Box midYellowBox = {
        .left = separatorPink,
        .top = topYellowBox.getBottom() + separatorPink,
        .width = yellowBoxWidth,
        .height = 162 * SIZE_FACTOR,
        .borderRadius = 10,
        .borderWidth = 3,
        .colour = COLOUR_YELLOW,
        .borderColour = COLOUR_BLACK,
    };
    StyledComponent::Box topGreyBox = {
        .left = separatorPink,
        .top = separatorPink,
        .width = yellowBoxWidth + 2 * separatorPink,
        .height = topYellowBox.height + midYellowBox.height + 3 * separatorPink,
        .colour = COLOUR_LIGHT_GREY,
    };
    StyledComponent::Box topBlackBox = {
        .left = 0,
        .top = 0,
        .width = topGreyBox.width + 2 * separatorPink,
        .height = topGreyBox.height + 2 * separatorPink,
        .borderRadius = 10,
        .colour = COLOUR_BLACK,
    };
    StyledComponent::Box gainBox = {
        .left = separatorBlue + borderSize,
        .top = (midYellowBox.height - midYellowBox.borderWidth * 2) / 2,
        .width = 80 * SIZE_FACTOR,
        .height = (midYellowBox.height - midYellowBox.borderWidth * 2) / 2,
        .colour = COLOUR_DARK_GREY,
    };
    StyledComponent::Box toneBox = {
        .left = gainBox.getRight() + separatorGreen,
        .top = gainBox.top,
        .width = gainBox.width,
        .height = gainBox.height,
        .colour = COLOUR_DARK_GREY,
    };
    StyledComponent::Box levelBox = {
        .left = toneBox.getRight() + separatorGreen,
        .top = gainBox.top,
        .width = toneBox.width,
        .height = toneBox.height,
        .colour = COLOUR_DARK_GREY,
    };
    const int labelSizeIncrease = 20;

    StyledComponent::Box gainLabelBox = {
        .left = gainBox.left - labelSizeIncrease / 2,
        .top = separatorPink,
        .width = gainBox.width + labelSizeIncrease,
        .height = gainBox.height - separatorPink,
        .colour = COLOUR_LIGHT_GREY,
    };
    StyledComponent::Box toneLabelBox = {
        .left = toneBox.left - labelSizeIncrease / 2,
        .top = separatorPink,
        .width = gainLabelBox.width,
        .height = gainLabelBox.height,
        .colour = COLOUR_LIGHT_GREY,
    };
    StyledComponent::Box levelLabelBox = {
        .left = levelBox.left - labelSizeIncrease / 2,
        .top = separatorPink,
        .width = gainLabelBox.width,
        .height = gainLabelBox.height,
        .colour = COLOUR_LIGHT_GREY,
    };

    topComponent.styles = topBlackBox;

    auto topGreyBoxComponent = std::make_shared<StyledComponent>(topGreyBox);
    auto topYellowBoxComponent = std::make_shared<StyledComponent>(topYellowBox);
    auto midYellowBoxComponent = std::make_shared<StyledComponent>(midYellowBox);
    auto gainBoxComponent = std::make_shared<StyledComponent>(gainBox);
    auto toneBoxComponent = std::make_shared<StyledComponent>(toneBox);
    auto levelBoxComponent = std::make_shared<StyledComponent>(levelBox);
    auto gainLabelBoxComponent = std::make_shared<StyledComponent>(gainLabelBox);
    auto toneLabelBoxComponent = std::make_shared<StyledComponent>(toneLabelBox);
    auto levelLabelBoxComponent = std::make_shared<StyledComponent>(levelLabelBox);
    
    const Font font("Arial", 30, 0);
    
    gainSlider = std::make_shared<Slider>();
    gainLabel = std::make_shared<Label>();
    gainLabel->setText(upper(GAIN_NAME), dontSendNotification);
    gainLabel->setJustificationType(Justification::centredTop);
    gainLabel->setFont(font);
    gainLabel->setColour(Label::textColourId, COLOUR_BLACK);
    gainValue = std::make_unique<SliderAttachment>(audioProcessor.treeState, GAIN_ID, *gainSlider);
    gainSlider->setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    gainSlider->setRange(0.0f, 10.0f, 0.1f);
    gainSlider->setPopupDisplayEnabled(true, true, this);
    gainSlider->setLookAndFeel(&dialLookAndFeel);

    toneSlider = std::make_shared<Slider>();
    toneLabel = std::make_shared<Label>();
    toneLabel->setText(upper(TONE_NAME), dontSendNotification);
    toneLabel->setJustificationType(Justification::centredTop);
    toneLabel->setFont(font);
    toneLabel->setColour(Label::textColourId, COLOUR_BLACK);
    toneValue = std::make_unique<SliderAttachment>(audioProcessor.treeState, TONE_ID, *toneSlider);
    toneSlider->setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    toneSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    toneSlider->setRange(0.0f, 10.0f, 0.1f);
    toneSlider->setPopupDisplayEnabled(true, true, this);
    toneSlider->setLookAndFeel(&dialLookAndFeel);

    outLevelSlider = std::make_shared<Slider>();
    outLevelLabel = std::make_shared<Label>();
    outLevelLabel->setText(upper(OUTPUT_NAME), dontSendNotification);
    outLevelLabel->setJustificationType(Justification::centredTop);
    outLevelLabel->setFont(font);
    outLevelLabel->setColour(Label::textColourId, COLOUR_BLACK);
    outLevelValue = std::make_unique<SliderAttachment>(audioProcessor.treeState, OUTPUT_ID, *outLevelSlider);
    outLevelSlider->setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    outLevelSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    outLevelSlider->setRange(0.f, 10.0f, 0.1f);
    outLevelSlider->setPopupDisplayEnabled(true, true, this);
    outLevelSlider->setLookAndFeel(&dialLookAndFeel);

    addAndMakeVisible(topComponent);
    addAndMakeVisible(*gainLabel);
    addAndMakeVisible(*toneLabel);
    addAndMakeVisible(*outLevelLabel);
    topComponent.addChild(topGreyBoxComponent);
    topGreyBoxComponent->addChild(topYellowBoxComponent);
    topGreyBoxComponent->addChild(midYellowBoxComponent);
    midYellowBoxComponent->addChild(gainLabelBoxComponent);
    midYellowBoxComponent->addChild(toneLabelBoxComponent);
    midYellowBoxComponent->addChild(levelLabelBoxComponent);
    midYellowBoxComponent->addChild(gainBoxComponent);
    midYellowBoxComponent->addChild(toneBoxComponent);
    midYellowBoxComponent->addChild(levelBoxComponent);
    gainLabelBoxComponent->addChild(gainLabel);
    toneLabelBoxComponent->addChild(toneLabel);
    levelLabelBoxComponent->addChild(outLevelLabel);
    gainBoxComponent->addChild(gainSlider);
    toneBoxComponent->addChild(toneSlider);
    levelBoxComponent->addChild(outLevelSlider);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 600);
}

ShittyAmpAudioProcessorEditor::~ShittyAmpAudioProcessorEditor()
{
    gainSlider->setLookAndFeel(nullptr);
    toneSlider->setLookAndFeel(nullptr);
    outLevelSlider->setLookAndFeel(nullptr);
}

//==============================================================================
void ShittyAmpAudioProcessorEditor::paint(Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void ShittyAmpAudioProcessorEditor::placeKnob(Slider *slider, int column, int row)
{
    const int rowSize = 100;
    const int columnSize = 100;
    slider->setBounds(column * columnSize, row * rowSize, columnSize * 0.8, rowSize * 0.8);
}

void ShittyAmpAudioProcessorEditor::resized()
{
    topComponent.setBounds(getLocalBounds());
}
