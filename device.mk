#
# Copyright (C) 2021-2023 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# AAPT
PRODUCT_AAPT_CONFIG := normal
PRODUCT_AAPT_PREF_CONFIG := xxhdpi

# Audio
QCV_FAMILY_SKUS := taro cape

PRODUCT_COPY_FILES += \
$(foreach DEVICE_SKU, $(QCV_FAMILY_SKUS), \
    $(LOCAL_PATH)/audio/mixer_paths.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio/sku_$(DEVICE_SKU)/mixer_paths_waipio_mtp.xml \
    $(LOCAL_PATH)/audio/audio_effects.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio/sku_$(DEVICE_SKU)/audio_effects.xml \
    $(LOCAL_PATH)/audio/resourcemanager.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio/sku_$(DEVICE_SKU)/resourcemanager_waipio_mtp.xml \
    $(LOCAL_PATH)/audio/sound_trigger_mixer_paths.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio/sku_$(DEVICE_SKU)/sound_trigger_mixer_paths.xml \
    $(LOCAL_PATH)/audio/sound_trigger_platform_info.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio/sku_$(DEVICE_SKU)/sound_trigger_platform_info.xml \
    )

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/audio/audio_effects.xml:$(TARGET_COPY_OUT_ODM)/etc/audio_effects.xml \
    $(LOCAL_PATH)/audio/backend_conf.xml:$(TARGET_COPY_OUT_ODM)/etc/backend_conf.xml \
    $(LOCAL_PATH)/audio/card-defs.xml:$(TARGET_COPY_OUT_VENDOR)/etc/card-defs.xml \
    $(LOCAL_PATH)/audio/microphone_characteristics.xml:$(TARGET_COPY_OUT_VENDOR)/etc/microphone_characteristics.xml \
    $(LOCAL_PATH)/audio/mixer_paths.xml:$(TARGET_COPY_OUT_ODM)/etc/mixer_paths.xml \
    $(LOCAL_PATH)/audio/resourcemanager.xml:$(TARGET_COPY_OUT_ODM)/etc/resourcemanager.xml \
    $(LOCAL_PATH)/audio/sound_trigger_mixer_paths.xml:$(TARGET_COPY_OUT_ODM)/etc/sound_trigger_mixer_paths.xml \
    $(LOCAL_PATH)/audio/sound_trigger_platform_info.xml:$(TARGET_COPY_OUT_ODM)/etc/sound_trigger_platform_info.xml \
    $(LOCAL_PATH)/audio/usecaseKvManager.xml:$(TARGET_COPY_OUT_ODM)/etc/usecaseKvManager.xml \
    $(LOCAL_PATH)/audio/usecaseKvManager.xml:$(TARGET_COPY_OUT_VENDOR)/etc/usecaseKvManager.xml

# Boot animation
TARGET_SCREEN_HEIGHT := 2772
TARGET_SCREEN_WIDTH := 1240

# Display
TARGET_USES_PREBUILT_HWC := true

# Display
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/display.xml:$(TARGET_COPY_OUT_VENDOR)/etc/displayconfig/display_id_4630946336619243649.xml \
    $(LOCAL_PATH)/configs/display.xml:$(TARGET_COPY_OUT_VENDOR)/etc/displayconfig/display_id_4630946336619243650.xml

# Fingerprint
$(call soong_config_set,qtidisplay,oplus_udfps,true)
$(call soong_config_set,surfaceflinger,udfps_lib,//hardware/oplus:libudfps_extension.oplus)

# Overlays
DEVICE_PACKAGE_OVERLAYS += \
    $(LOCAL_PATH)/overlay-lineage

PRODUCT_PACKAGES += \
    OPlusFrameworksResTarget \
    OPlusSettingsProviderResTarget \
    OPlusSettingsResTarget \
    OPlusSystemUIResTarget

# Power
$(call soong_config_set,qtipower,mode_ext_lib,power-ext-oplus)

# Leds
PRODUCT_PACKAGES += \
    android.hardware.light@2.0-service.oplus

# Soong namespaces
PRODUCT_SOONG_NAMESPACES += \
    $(LOCAL_PATH)

# Inherit from the common OEM chipset makefile.
$(call inherit-product, device/oneplus/sm8450-common/common.mk)

# Inherit from the proprietary files makefile.
$(call inherit-product, vendor/realme/senna/senna-vendor.mk)
