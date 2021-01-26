#include <JuceHeader.h>
#include "StyledComponent.h"


StyledComponent::StyledComponent()
{
}

StyledComponent::StyledComponent(Box stylesToUse)
    : styles(stylesToUse)
{
    setInterceptsMouseClicks(false, true);
}

void StyledComponent::addChild(std::shared_ptr<Component> componentPtr)
{
    children.push_back(componentPtr);
    addAndMakeVisible(*componentPtr);
}

StyledComponent::~StyledComponent()
{
}

void StyledComponent::paint (juce::Graphics& g)
{
    g.setColour(styles.colour);
    Rectangle<int> rect = Rectangle<int>(styles.left, styles.top, styles.width, styles.height);
    //if (styles.borderRadius == 0)
    {
        g.fillRect(rect);
        if (styles.borderWidth != 0)
        {
            g.setColour(styles.borderColour);
            g.drawRect(rect, styles.borderWidth);
        }
    }
    /*
    else
    {
        Rectangle<float> floatLocalBounds = localBounds.getFloatEquivalent();
        float borderRadius = static_cast<float>(styles.borderRadius);
        g.fillRoundedRectangle(floatLocalBounds, borderRadius);
        if (styles.borderWidth != 0)
        {
            g.setColour(styles.borderColour);
            g.drawRoundedRectangle(floatLocalBounds, borderRadius, styles.borderWidth);
        }
    }
    */
}

void StyledComponent::resized()
{
    Rectangle<int> bounds = getLocalBounds()
        .withLeft(styles.left)
        .withTop(styles.top)
        .withWidth(styles.width)
        .withHeight(styles.height)
        .reduced(styles.borderWidth);
    for (int i = 0; i < children.size(); i++)
    {
        children[i]->setBounds(bounds);
    }
}
