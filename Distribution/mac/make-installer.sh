#!/bin/sh
set -e

# Guard
read -p "I will user the version stored in the .jucer file - have you already updated that? " -n 1 -r
echo    # (optional) move to a new line
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
    echo "Cancelling"
    exit 1
fi

# Inspired on a script from the juce forum -> https://forum.juce.com/t/vst-installer/16654/15
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
JUCERFILE="${DIR}/../../IgnitionGuitarAmp.jucer"

shortname="IgnitionGuitarAmp"
name="com.thmstudio.${shortname}"

version=`cat "${JUCERFILE}" | tq 'JUCERPROJECT' -a 'version'`
short_version=`echo "${version}" | cut -f 1,2 -d'.' -`

function fail {
    printf '%s\n' "$1" >&2  ## Send message to stderr. Exclude >&2 if you don't want it that way.
    exit "${2-1}"  ## Return a code specified by $2 or 1 by default.
}

# Make local folders for each plugin version & copy plugins from /Library to local folders

AUFILE="${HOME}/Library/Audio/Plug-Ins/Components/${shortname}.component"
if test -d "${AUFILE}"; then
    mkdir au/
    echo "Copying ${AUFILE}..."
    cp -R "${AUFILE}" au
else
    fail "${AUFILE} does not exist"
fi

VST3FILE="${HOME}/Library/Audio/Plug-Ins/VST3/${shortname}.vst3"
if test -d "${VST3FILE}"; then
    mkdir vst3/
    echo "Copying ${VST3FILE}..."
    cp -R "${VST3FILE}" vst3
else
    fail "${VST3FILE} does not exist"
fi

au_size=`du -ks ${AUFILE} | cut -f1`
vst3_size=`du -ks ${VST3FILE} | cut -f1`

# Perform AAX signing step.
# Not needed since I'm not publishing AAX
#/Applications/PACEAntiPiracy/Eden/Fusion/Versions/3/bin/wraptool sign --verbose --account XXXXX --password XXXXX --wcguid XXXX-XXX-XXX-XXXX --signid "Developer ID Application: Audio Damage, Inc. (XXXXXXX)" --in "./aax/Discord4.aaxplugin"

# Create flat installers for each plugin.
pkgbuild --analyze --root "./vst3/" "${name}.vst3.plist"
pkgbuild --analyze --root "./au/" "${name}.au.plist"
pkgbuild --root "./vst3/" --component-plist "./${name}.vst3.plist" --identifier "${name}.vst3" --version $version --install-location "/Library/Audio/Plug-Ins/VST3" "${name}.vst3.pkg"
pkgbuild --root "./au/" --component-plist "./${name}.au.plist" --identifier "${name}.au" --version $version --install-location "/Library/Audio/Plug-Ins/Components" "${name}.au.pkg"

# Fill up stuff in distribution template
sed 's/{short_version}/'"${short_version}/g" distribution-template.xml > tmp-1.xml
sed 's/{version}/'"${version}/g" tmp-1.xml > tmp-2.xml
sed 's/{au_size}/'"${au_size}/g" tmp-2.xml > tmp-3.xml
sed 's/{vst3_size}/'"${vst3_size}/g" tmp-3.xml > Distribution.xml

# Make folder for this package
installer_dir="${DIR}/out"
mkdir -p "${installer_dir}"

# Package them together to a “thick” installer.
# Create final product from local assets.
# productbuild --distribution "./Distribution.xml" --package-path "./" --resources "./Resources" --sign "Developer ID Installer: Audio Damage, Inc." "OSX_${name}_${version}.pkg"
productbuild --distribution "./Distribution.xml" --package-path "./" --resources "./Resources" "${installer_dir}/${shortname}_${version}.pkg"

# Clean up
rm "${name}.vst3.plist"
rm "${name}.au.plist"
rm "${name}.vst3.pkg"
rm "${name}.au.pkg"
rm tmp-*.xml
rm Distribution.xml
rm -r "./vst3"
rm -r "./au"
