#include "MainFont.h"

MainFont::MainFont()
    : regular(Font(
          Typeface::createSystemTypefaceFor(BinaryData::JorgeLuisCafeCafe_ttf, BinaryData::JorgeLuisCafeCafe_ttfSize)))
    , lightItalic(Font(
          Typeface::createSystemTypefaceFor(BinaryData::JorgeLuisCafeLightItalic_ttf, BinaryData::JorgeLuisCafeLightItalic_ttfSize)))
    , light(Font(
          Typeface::createSystemTypefaceFor(BinaryData::JorgeLuisCafeLight_ttf, BinaryData::JorgeLuisCafeLight_ttfSize)))
    , italic(Font(
          Typeface::createSystemTypefaceFor(BinaryData::JorgeLuisCafeItalic_ttf, BinaryData::JorgeLuisCafeItalic_ttfSize)))
    , bold(Font(
          Typeface::createSystemTypefaceFor(BinaryData::JorgeLuisCafeBold_ttf, BinaryData::JorgeLuisCafeBold_ttfSize)))
    , boldItalic(Font(
          Typeface::createSystemTypefaceFor(BinaryData::JorgeLuisCafeBoldItalic_ttf, BinaryData::JorgeLuisCafeBoldItalic_ttfSize)))
{
}

Font& MainFont::getRegular()
{
    return regular;
}

Font& MainFont::getLightItalic()
{
    return lightItalic;
}

Font& MainFont::getLight()
{
    return light;
}

Font& MainFont::getItalic()
{
    return italic;
}

Font& MainFont::getBold()
{
    return bold;
}

Font& MainFont::getBoldItalic()
{
    return boldItalic;
}
