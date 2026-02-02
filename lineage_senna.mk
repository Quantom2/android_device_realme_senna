#
# Copyright (C) 2021-2023 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit_only.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit from senna device
$(call inherit-product, device/realme/senna/device.mk)

# Inherit some common Lineage stuff.
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

PRODUCT_NAME := lineage_senna
PRODUCT_DEVICE := senna
PRODUCT_MANUFACTURER := realme
PRODUCT_BRAND := realme
PRODUCT_MODEL := RMX3706

PRODUCT_GMS_CLIENTID_BASE := android-realme

# Disable adb secure while we developing
WITH_ADB_INSECURE := true

PRODUCT_BUILD_PROP_OVERRIDES += \
    BuildDesc="RMX3706-user 15 AP3A.240617.008 T.1d9a49b_20be2-8d release-keys" \
    BuildFingerprint=realme/RMX3706/RE5860:15/AP3A.240617.008/T.1d9a49b_20be2-8d:user/release-keys \
    DeviceName=RE5860 \
    DeviceProduct=RMX3706 \
    SystemDevice=RE5860 \
    SystemName=RMX3706
