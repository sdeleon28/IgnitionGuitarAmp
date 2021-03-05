#include "ThmAmpText.h"
#include "MainFont.h"

ThmAmpText::ThmAmpText()
{
    MainFont mainFont;
    const Font thmFont = mainFont.getRegular().withHeight(44);
    const Font ampFont = mainFont.getItalic().withHeight(22);
    Range<int> thmRange(0, 3);
    Range<int> ampRange(4, 12);
    AttributedString attributedString("THM ignition");
    attributedString.setFont(thmRange, thmFont);
    attributedString.setFont(ampRange, ampFont);
    attributedString.setJustification(Justification::centredLeft);
    layout.createLayout(attributedString, 200);
}

void ThmAmpText::paint (Graphics& g)
{
    Rectangle<int> bounds = getLocalBounds().withLeft(40);
    layout.draw(g, bounds.toFloat());
}
