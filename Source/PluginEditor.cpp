#include "PluginProcessor.h"
#include "PluginEditor.h"

#define SIZE_FACTOR 0.5f
#define SEPARATOR_PINK 20
#define SEPARATOR_GREEN 40
#define SEPARATOR_BLUE 88
#define COLOUR_BLACK Colour(28, 28, 28)
#define COLOUR_YELLOW Colour(249, 226, 85)
#define COLOUR_LIGHT_GREY Colour(142, 142, 142)
#define COLOUR_DARK_GREY Colour(58, 58, 58)

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
    gainSlider.setLookAndFeel(&dialLookAndFeel);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(gainSlider);

    toneLabel.setText(TONE_NAME, dontSendNotification);
    toneLabel.attachToComponent(&toneSlider, false);
    toneValue = std::make_unique<SliderAttachment>(audioProcessor.treeState, TONE_ID, toneSlider);
    toneSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    toneSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    toneSlider.setRange(0.0f, 10.0f, 0.1f);
    toneSlider.setPopupDisplayEnabled(true, true, this);
    toneSlider.setLookAndFeel(&dialLookAndFeel);
    addAndMakeVisible(toneLabel);
    addAndMakeVisible(toneSlider);

    outLevelLabel.setText(OUTPUT_NAME, dontSendNotification);
    outLevelLabel.attachToComponent(&outLevelSlider, false);
    outLevelValue = std::make_unique<SliderAttachment>(audioProcessor.treeState, OUTPUT_ID, outLevelSlider);
    outLevelSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    outLevelSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    outLevelSlider.setRange(0.f, 10.0f, 0.1f);
    outLevelSlider.setPopupDisplayEnabled(true, true, this);
    outLevelSlider.setLookAndFeel(&dialLookAndFeel);
    addAndMakeVisible(outLevelLabel);
    addAndMakeVisible(outLevelSlider);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 600);
}

ShittyAmpAudioProcessorEditor::~ShittyAmpAudioProcessorEditor()
{
    gainSlider.setLookAndFeel(nullptr);
    toneSlider.setLookAndFeel(nullptr);
    outLevelSlider.setLookAndFeel(nullptr);
}

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

void paintBox(Box &box, Graphics &g)
{
    g.setColour(box.colour);
    Rectangle<float> rect = Rectangle<float>(box.left, box.top, box.width, box.height);
    if (box.cornerSize == 0)
    {
        g.fillRect(rect);
        if (box.borderThickness != 0)
        {
            g.setColour(box.borderColour);
            g.drawRect(rect, box.borderThickness);
        }
    }
    else
    {
        g.fillRoundedRectangle(rect, box.cornerSize);
        if (box.borderThickness != 0)
        {
            g.setColour(box.borderColour);
            g.drawRoundedRectangle(rect, box.cornerSize, box.borderThickness);
        }
    }
}

//==============================================================================
void ShittyAmpAudioProcessorEditor::paint(Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    const float borderSize = 3;
    const float separatorPink = SEPARATOR_PINK * SIZE_FACTOR;
    const float separatorBlue = SEPARATOR_BLUE * SIZE_FACTOR;
    const float separatorGreen = SEPARATOR_GREEN * SIZE_FACTOR;
    const float yellowBoxWidth = (504 + 2 * borderSize) * SIZE_FACTOR;

    Box topYellowBox = {
        .left = 2 * separatorPink,
        .top = 2 * separatorPink,
        .width = yellowBoxWidth,
        .height = 58 * SIZE_FACTOR,
        .cornerSize = 10,
        .borderThickness = 3,
        .colour = COLOUR_YELLOW,
        .borderColour = COLOUR_BLACK,
    };
    Box midYellowBox = {
        .left = 2 * separatorPink,
        .top = topYellowBox.getBottom() + separatorPink,
        .width = yellowBoxWidth,
        .height = 162 * SIZE_FACTOR,
        .cornerSize = 10,
        .borderThickness = 3,
        .colour = COLOUR_YELLOW,
        .borderColour = COLOUR_BLACK,
    };
    Box topGreyBox = {
        .left = separatorPink,
        .top = separatorPink,
        .width = yellowBoxWidth + 2 * separatorPink,
        .height = topYellowBox.height + midYellowBox.height + 3 * separatorPink,
        .colour = COLOUR_LIGHT_GREY,
    };
    Box topBlackBox = {
        .left = 0,
        .top = 0,
        .width = topGreyBox.width + 2 * separatorPink,
        .height = topGreyBox.height + 2 * separatorPink,
        .cornerSize = 10,
        .colour = COLOUR_BLACK,
    };
    Box bottomYellowBox = {
        .left = 2 * separatorPink,
        .top = topBlackBox.getBottom() + 2 * separatorPink,
        .width = yellowBoxWidth,
        .height = 240 * SIZE_FACTOR,
        .cornerSize = 10,
        .borderThickness = 3,
        .colour = COLOUR_YELLOW,
        .borderColour = COLOUR_BLACK,
    };
    Box bottomGreyBox = {
        .left = separatorPink,
        .top = topBlackBox.getBottom() + separatorPink,
        .width = yellowBoxWidth + 2 * separatorPink,
        .height = bottomYellowBox.height + 2 * separatorPink,
        .colour = COLOUR_LIGHT_GREY,
    };
    Box bottomBlackBox = {
        .left = 0,
        .top = topBlackBox.getBottom(),
        .width = topGreyBox.width + 2 * separatorPink,
        .height = topGreyBox.height + 2 * separatorPink,
        .cornerSize = 10,
        .colour = COLOUR_BLACK,
    };
    Box gainBox = {
        .left = midYellowBox.left + separatorBlue + borderSize,
        .top = midYellowBox.top,
        .width = 80 * SIZE_FACTOR,
        .height = midYellowBox.height,
        .colour = COLOUR_DARK_GREY,
    };
    Box toneBox = {
        .left = gainBox.getRight() + separatorGreen,
        .top = gainBox.top,
        .width = gainBox.width,
        .height = gainBox.height,
        .colour = COLOUR_DARK_GREY,
    };
    Box levelBox = {
        .left = toneBox.getRight() + separatorGreen,
        .top = toneBox.top,
        .width = toneBox.width,
        .height = toneBox.height,
        .colour = COLOUR_DARK_GREY,
    };

    paintBox(topBlackBox, g);
    paintBox(bottomBlackBox, g);
    paintBox(topGreyBox, g);
    paintBox(bottomGreyBox, g);
    paintBox(topYellowBox, g);
    paintBox(midYellowBox, g);
    paintBox(bottomYellowBox, g);
    paintBox(gainBox, g);
    paintBox(toneBox, g);
    paintBox(levelBox, g);
}

void ShittyAmpAudioProcessorEditor::placeKnob(Slider *slider, int column, int row)
{
    const int rowSize = 100;
    const int columnSize = 100;
    slider->setBounds(column * columnSize, row * rowSize, columnSize * 0.8, rowSize * 0.8);
}

void ShittyAmpAudioProcessorEditor::resized()
{
    //Rectangle<int> bounds = getLocalBounds();

    // TODO: Add these back
    // First row
    //placeKnob(&gainSlider, 0, 1);
    //placeKnob(&toneSlider, 1, 1);
    //placeKnob(&outLevelSlider, 2, 1);
}
