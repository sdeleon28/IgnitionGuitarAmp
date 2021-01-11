/*
  ==============================================================================

    Utils.h
    Created: 6 Dec 2020 10:24:26pm
    Author:  Santiago de Leon

  ==============================================================================
*/

#pragma once

inline std::unique_ptr<InputStream> createAssetInputStream(const char *resourcePath)
{
#if JUCE_ANDROID
    ZipFile apkZip(File::getSpecialLocation(File::invokedExecutableFile));
    return std::unique_ptr<InputStream>(
        apkZip.createStreamForEntry(apkZip.getIndexOfFileName("assets/" + String(resourcePath))));
#else
#if JUCE_IOS
    auto assetsDir = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getChildFile("Assets");
#elif JUCE_MAC
    auto assetsDir = File::getSpecialLocation(File::currentExecutableFile)
                         .getParentDirectory()
                         .getParentDirectory()
                         .getChildFile("Resources");
#endif

    auto resourceFile = assetsDir.getChildFile(resourcePath);
    jassert(resourceFile.existsAsFile());

    return resourceFile.createInputStream();
#endif
}