#!/bin/sh

cd ../../../../

if [ -d "$HOME/bin" ] ; then
    PATH="$HOME/bin:$PATH"
fi

source build/envsetup.sh

# SDM commits
# see https://review.lineageos.org/c/LineageOS/android_hardware_qcom_display/+/432432 chain
repopick -f 432432 -P hardware/qcom-caf/sm8450/display
repopick -f 474474 -P hardware/qcom-caf/sm8450/display
repopick -f 432430 -P hardware/qcom-caf/sm8450/display
repopick -f 432431 -P hardware/qcom-caf/sm8450/display

# Iris5 patch
cd hardware/qcom-caf/sm8450/display
patch -p1 < ../../../../device/realme/senna/patches/iris5.patch

cd ../../../../

# Acdbdata fix
# https://review.lineageos.org/c/LineageOS/android_vendor_qcom_opensource_agm/+/417220
repopick -f 417220 -P hardware/qcom-caf/sm8450/audio/agm

# Camera lvacfs fix
# https://review.lineageos.org/c/LineageOS/android_hardware_qcom_audio-ar/+/466887
repopick -f 466887 -P hardware/qcom-caf/sm8450/audio/primary-hal


# Oplus camera fixes (disabled)
# https://review.lineageos.org/q/topic:%22oplus-camera%22

# Add oplus fwb stubs
# https://review.lineageos.org/c/LineageOS/android_frameworks_base/+/458795
#repopick -f 458795 -P frameworks/base

# Add oplus packages to allowed list
# https://review.lineageos.org/c/LineageOS/android_build_soong/+/449821
repopick -f 449821 -P build/soong

# add support in frameworks/av
# https://review.lineageos.org/c/LineageOS/android_frameworks_av/+/455598
repopick -f 455598 -P frameworks/av

# Add support to set vendor tag package 
# https://review.lineageos.org/c/LineageOS/android_frameworks_av/+/455597
repopick -f 455597 -P frameworks/av

# Rebase pjgowtham's usefull commits
cd frameworks/base
git remote add pj https://github.com/pjgowtham/android_frameworks_base
git fetch pj
git checkout pj/lineage-23.2
git rebase github/lineage-23.2

cd vendor/oneplus/sm8450-common
git checkout 37cb2da89559e704b14086c40c0faa7b1cf0b248 --force
cd ../../../

cd device/oneplus/sm8450-common/vibrator/aidl

./patch_framework.sh

echo "> Done!"