#pragma once

#include <JuceHeader.h>

class MainFont
{
private:
    Font regular;
    Font lightItalic;
    Font light;
    Font italic;
    Font bold;
    Font boldItalic;

public:
    MainFont();
    Font& getRegular();
    Font& getLightItalic();
    Font& getLight();
    Font& getItalic();
    Font& getBold();
    Font& getBoldItalic();
};
