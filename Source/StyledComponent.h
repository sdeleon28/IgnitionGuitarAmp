#pragma once

#include <JuceHeader.h>

#define SIZE_FACTOR 1
#define SEPARATOR_PINK 20
#define SEPARATOR_GREEN 44
#define SEPARATOR_BLUE 88
#define COLOUR_BLACK Colour(28, 28, 28)
#define COLOUR_YELLOW Colour(249, 226, 85)
#define COLOUR_LIGHT_GREY Colour(142, 142, 142)
#define COLOUR_DARK_GREY Colour(58, 58, 58)

class StyledComponent  : public juce::Component
{
public:
    struct View {
        Colour backgroundColor;
        Colour borderColor = COLOUR_BLACK;
        int borderRadius = 0;
        int borderWidth = 0;
        int height;
        int left;
        int top;
        int width;

        int getBottom()
        {
            return top + height;
        }
        
        int getRight()
        {
            return left + width;
        }
    };

    StyledComponent();
    StyledComponent(View stylesToUse);
    ~StyledComponent() override;

    void addChild(std::shared_ptr<Component> child);
    void paint (juce::Graphics&) override;
    void resized() override;

    View styles;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StyledComponent)
    std::vector<std::shared_ptr<Component>> children;
};
