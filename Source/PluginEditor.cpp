#include <algorithm>
#include <string>
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ThmAmpText.h"

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

    const int borderWidth = 3;
    const int separatorPink = SEPARATOR_PINK * SIZE_FACTOR;
    const int separatorBlue = SEPARATOR_BLUE * SIZE_FACTOR;
    const int separatorGreen = SEPARATOR_GREEN * SIZE_FACTOR;
    const int yellowBoxWidth = (480 + 2 * borderWidth) * SIZE_FACTOR;

    StyledComponent::View topYellowBox = {
        .backgroundColor = COLOUR_YELLOW,
        .borderColor = COLOUR_BLACK,
        .borderRadius = 10,
        .borderWidth = borderWidth,
        .height = 58 * SIZE_FACTOR,
        .left = separatorPink,
        .top = separatorPink,
        .width = yellowBoxWidth,
    };
    StyledComponent::View midYellowBox = {
        .backgroundColor = COLOUR_YELLOW,
        .borderColor = COLOUR_BLACK,
        .borderRadius = 10,
        .borderWidth = 3,
        .height = (162 + 2 * borderWidth) * SIZE_FACTOR,
        .left = separatorPink,
        .top = topYellowBox.getBottom() + separatorPink,
        .width = yellowBoxWidth,
    };
    StyledComponent::View topGreyBox = {
        .backgroundColor = COLOUR_LIGHT_GREY,
        .height = topYellowBox.height + midYellowBox.height + 3 * separatorPink,
        .left = separatorPink,
        .top = separatorPink,
        .width = yellowBoxWidth + 2 * separatorPink,
    };
    StyledComponent::View topBlackBox = {
        .backgroundColor = COLOUR_BLACK,
        .borderRadius = 10,
        .height = topGreyBox.height + 2 * separatorPink,
        .left = 0,
        .top = 0,
        .width = topGreyBox.width + 2 * separatorPink,
    };

    const int labelHeight = 30;
    const int knobHeight = midYellowBox.height - 2 * midYellowBox.borderWidth - 3 * separatorPink - labelHeight;

    StyledComponent::View gainBox = {
        .height = knobHeight,
        .left = separatorBlue,
        .top = 2 * separatorPink + labelHeight,
        .width = knobHeight,
    };
    StyledComponent::View toneBox = {
        .height = gainBox.height,
        .left = gainBox.getRight() + separatorGreen,
        .top = gainBox.top,
        .width = gainBox.width,
    };
    StyledComponent::View levelBox = {
        .height = toneBox.height,
        .left = toneBox.getRight() + separatorGreen,
        .top = gainBox.top,
        .width = toneBox.width,
    };
    const int labelSizeIncrease = 20;

    StyledComponent::View gainLabelBox = {
        .height = labelHeight,
        .left = gainBox.left - labelSizeIncrease / 2,
        .top = separatorPink,
        .width = gainBox.width + labelSizeIncrease,
    };
    StyledComponent::View toneLabelBox = {
        .height = labelHeight,
        .left = toneBox.left - labelSizeIncrease / 2,
        .top = separatorPink,
        .width = gainLabelBox.width,
    };
    StyledComponent::View levelLabelBox = {
        .height = labelHeight,
        .left = levelBox.left - labelSizeIncrease / 2,
        .top = separatorPink,
        .width = gainLabelBox.width,
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
    
    auto thmAmpText = std::make_shared<ThmAmpText>();

    const Font sliderLabelFont = mainFont.getBold().withHeight(30);

    gainSlider = std::make_shared<Slider>();
    gainLabel = std::make_shared<Label>();
    gainLabel->setText(upper(GAIN_NAME), dontSendNotification);
    gainLabel->setJustificationType(Justification::centredTop);
    gainLabel->setFont(sliderLabelFont);
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
    toneLabel->setFont(sliderLabelFont);
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
    outLevelLabel->setFont(sliderLabelFont);
    outLevelLabel->setColour(Label::textColourId, COLOUR_BLACK);
    outLevelValue = std::make_unique<SliderAttachment>(audioProcessor.treeState, OUTPUT_ID, *outLevelSlider);
    outLevelSlider->setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    outLevelSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    outLevelSlider->setRange(0.f, 10.0f, 0.1f);
    outLevelSlider->setPopupDisplayEnabled(true, true, this);
    outLevelSlider->setLookAndFeel(&dialLookAndFeel);

    addAndMakeVisible(topComponent);
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
    topYellowBoxComponent->addChild(thmAmpText);
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

void ShittyAmpAudioProcessorEditor::resized()
{
    topComponent.setBounds(getLocalBounds());
}
