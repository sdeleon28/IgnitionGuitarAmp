#include "MainFont.h"

MainFont::MainFont()
    : regular(Font(
          Typeface::createSystemTypefaceFor(BinaryData::LatoRegular_ttf, BinaryData::LatoRegular_ttfSize)))
    , lightItalic(Font(
          Typeface::createSystemTypefaceFor(BinaryData::LatoLightItalic_ttf, BinaryData::LatoLightItalic_ttfSize)))
    , light(Font(
          Typeface::createSystemTypefaceFor(BinaryData::LatoLight_ttf, BinaryData::LatoLight_ttfSize)))
    , italic(Font(
          Typeface::createSystemTypefaceFor(BinaryData::LatoItalic_ttf, BinaryData::LatoItalic_ttfSize)))
    , bold(Font(
          Typeface::createSystemTypefaceFor(BinaryData::LatoBold_ttf, BinaryData::LatoBold_ttfSize)))
    , boldItalic(Font(
          Typeface::createSystemTypefaceFor(BinaryData::LatoBoldItalic_ttf, BinaryData::LatoBoldItalic_ttfSize)))
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
