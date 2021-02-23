/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   JorgeLuisCafeBoldItalic_ttf;
    const int            JorgeLuisCafeBoldItalic_ttfSize = 36404;

    extern const char*   JorgeLuisCafeBold_ttf;
    const int            JorgeLuisCafeBold_ttfSize = 34484;

    extern const char*   JorgeLuisCafeCafe_ttf;
    const int            JorgeLuisCafeCafe_ttfSize = 32020;

    extern const char*   JorgeLuisCafeItalic_ttf;
    const int            JorgeLuisCafeItalic_ttfSize = 33756;

    extern const char*   JorgeLuisCafeLightItalic_ttf;
    const int            JorgeLuisCafeLightItalic_ttfSize = 34076;

    extern const char*   JorgeLuisCafeLight_ttf;
    const int            JorgeLuisCafeLight_ttfSize = 32552;

    extern const char*   dial_png;
    const int            dial_pngSize = 3609;

    extern const char*   cabsim_wav;
    const int            cabsim_wavSize = 72044;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 8;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
