#pragma once
#include <JuceHeader.h>

class ThmAmpText : public Component
{
    public:
        ThmAmpText();
        void paint (Graphics& g);
    private:
        TextLayout layout;
};
