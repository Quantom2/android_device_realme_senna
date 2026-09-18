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
    $(LOCAL_PATH)/audio/resourcemanager.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio/sku_$(DEVICE_SKU)/resourcemanager_waipio_mtp.xml \
    )

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/audio/backend_conf.xml:$(TARGET_COPY_OUT_ODM)/etc/backend_conf.xml \
    $(LOCAL_PATH)/audio/mixer_paths.xml:$(TARGET_COPY_OUT_ODM)/etc/mixer_paths.xml \
    $(LOCAL_PATH)/audio/mixer_paths.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio/sku_ukee/mixer_paths_ukee_mtp.xml \
    $(LOCAL_PATH)/audio/resourcemanager.xml:$(TARGET_COPY_OUT_ODM)/etc/resourcemanager.xml \
    $(LOCAL_PATH)/audio/resourcemanager.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio/sku_ukee/resourcemanager_ukee_mtp.xml \
    $(LOCAL_PATH)/audio/usecaseKvManager.xml:$(TARGET_COPY_OUT_ODM)/etc/usecaseKvManager.xml \
    $(LOCAL_PATH)/audio/usecaseKvManager.xml:$(TARGET_COPY_OUT_VENDOR)/etc/usecaseKvManager.xml

# Boot animation
TARGET_SCREEN_HEIGHT := 2772
TARGET_SCREEN_WIDTH := 1240

# Display
$(call soong_config_set,qtidisplay,pxlw_vendor_namespace,vendor/realme/senna)
$(call soong_config_set,qtidisplay,pxlw_hw_iris5,true)

# Display
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/display.xml:$(TARGET_COPY_OUT_VENDOR)/etc/displayconfig/display_id_4630946336619243649.xml \
    $(LOCAL_PATH)/configs/display.xml:$(TARGET_COPY_OUT_VENDOR)/etc/displayconfig/display_id_4630946336619243650.xml

# Fingerprint
$(call soong_config_set_bool,qtidisplay,oplus_udfps,true)
$(call soong_config_set,surfaceflinger,udfps_lib,//hardware/oplus:libudfps_extension.oplus)

# Overlays
DEVICE_PACKAGE_OVERLAYS += \
    $(LOCAL_PATH)/overlay-lineage

PRODUCT_PACKAGES += \
    OPlusFrameworksResTarget \
    OPlusSettingsProviderResTarget \
    OPlusSettingsResTarget \
    OPlusSystemUIResTarget

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/recovery/root/odm/22624/odm/firmware/fastchg/22624/charging_hyper_mode_config.txt:$(TARGET_COPY_OUT_ODM)/22624/odm/firmware/fastchg/22624/charging_hyper_mode_config.txt \
    $(LOCAL_PATH)/recovery/root/odm/22624/odm/firmware/fastchg/22624/bms_heating_config.txt:$(TARGET_COPY_OUT_ODM)/22624/odm/firmware/fastchg/22624/bms_heating_config.txt \
    $(LOCAL_PATH)/recovery/root/odm/22624/odm/firmware/fastchg/22624/charging_thermal_config_default.txt:$(TARGET_COPY_OUT_ODM)/22624/odm/firmware/fastchg/22624/charging_thermal_config_default.txt \
    $(LOCAL_PATH)/recovery/root/odm/22624/odm/firmware/fastchg/charging_thermal_config_default.txt:$(TARGET_COPY_OUT_ODM)/22624/odm/firmware/fastchg/charging_thermal_config_default.txt \
    $(LOCAL_PATH)/recovery/root/odm/22624/odm/firmware/fastchg/smart_chg_config_table.txt:$(TARGET_COPY_OUT_ODM)/22624/odm/firmware/fastchg/smart_chg_config_table.txt \
    $(LOCAL_PATH)/recovery/root/odm/22625/odm/firmware/fastchg/22625/charging_hyper_mode_config.txt:$(TARGET_COPY_OUT_ODM)/22625/odm/firmware/fastchg/22625/charging_hyper_mode_config.txt \
    $(LOCAL_PATH)/recovery/root/odm/22625/odm/firmware/fastchg/22625/bms_heating_config.txt:$(TARGET_COPY_OUT_ODM)/22625/odm/firmware/fastchg/22625/bms_heating_config.txt \
    $(LOCAL_PATH)/recovery/root/odm/22625/odm/firmware/fastchg/22625/charging_thermal_config_default.txt:$(TARGET_COPY_OUT_ODM)/22625/odm/firmware/fastchg/22625/charging_thermal_config_default.txt \
    $(LOCAL_PATH)/recovery/root/odm/22625/odm/firmware/fastchg/charging_thermal_config_default.txt:$(TARGET_COPY_OUT_ODM)/22625/odm/firmware/fastchg/charging_thermal_config_default.txt \
    $(LOCAL_PATH)/recovery/root/odm/22625/odm/firmware/fastchg/smart_chg_config_table.txt:$(TARGET_COPY_OUT_ODM)/22625/odm/firmware/fastchg/smart_chg_config_table.txt \
    $(LOCAL_PATH)/recovery/root/odm/226B2/odm/firmware/fastchg/226B2/charging_hyper_mode_config.txt:$(TARGET_COPY_OUT_ODM)/226B2/odm/firmware/fastchg/226B2/charging_hyper_mode_config.txt \
    $(LOCAL_PATH)/recovery/root/odm/226B2/odm/firmware/fastchg/226B2/bms_heating_config.txt:$(TARGET_COPY_OUT_ODM)/226B2/odm/firmware/fastchg/226B2/bms_heating_config.txt \
    $(LOCAL_PATH)/recovery/root/odm/226B2/odm/firmware/fastchg/226B2/charging_thermal_config_default.txt:$(TARGET_COPY_OUT_ODM)/226B2/odm/firmware/fastchg/226B2/charging_thermal_config_default.txt \
    $(LOCAL_PATH)/recovery/root/odm/226B2/odm/firmware/fastchg/charging_thermal_config_default.txt:$(TARGET_COPY_OUT_ODM)/226B2/odm/firmware/fastchg/charging_thermal_config_default.txt \
    $(LOCAL_PATH)/recovery/root/odm/226B2/odm/firmware/fastchg/smart_chg_config_table.txt:$(TARGET_COPY_OUT_ODM)/226B2/odm/firmware/fastchg/smart_chg_config_table.txt
# Power
$(call soong_config_set,qtipower,mode_ext_lib,power-ext-oplus)

# Leds
PRODUCT_PACKAGES += \
    android.hardware.light-service.senna

# IR
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.consumerir.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.consumerir.xml
PRODUCT_PACKAGES += \
    android.hardware.ir-service.oplus

# Gestures
$(call soong_config_set,OPLUS_LINEAGE_TOUCH_HAL,INCLUDE_DIR,$(LOCAL_PATH)/touch/include)


# Soong namespaces
PRODUCT_SOONG_NAMESPACES += \
    $(LOCAL_PATH) \
    hardware/pixelworks/interfaces
    
# Inherit from the common OEM chipset makefile.
$(call inherit-product, device/oneplus/sm8450-common/common.mk)

# Inherit from the proprietary files makefile.
$(call inherit-product, vendor/realme/senna/senna-vendor.mk)
