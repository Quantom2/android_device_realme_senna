#
# Copyright (C) 2021-2023 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Include the common OEM chipset BoardConfig.
include device/oneplus/sm8450-common/BoardConfigCommon.mk

DEVICE_PATH := device/realme/senna

# Kernel
TARGET_KERNEL_CONFIG += vendor/oplus/senna.config

# Display
TARGET_SCREEN_DENSITY := 450

# Leds
DEVICE_FRAMEWORK_COMPATIBILITY_MATRIX_FILE += $(DEVICE_PATH)/framework_compatibility_matrix.xml
BOARD_VENDOR_SEPOLICY_DIRS += $(DEVICE_PATH)/sepolicy/vendor

# Partitions
ifeq (,$(filter true, $(WITHOUT_RESERVED_SIZE) $(WITH_GMS)))
BOARD_PRODUCTIMAGE_PARTITION_RESERVED_SIZE := 900000000
endif
BOARD_ONEPLUS_DYNAMIC_PARTITIONS_SIZE := 7247757310 # ( BOARD_SUPER_PARTITION_SIZE / 2) - 2B?
BOARD_SUPER_PARTITION_SIZE := 14495514624

# Properties
TARGET_VENDOR_PROP += $(DEVICE_PATH)/vendor.prop

# Add for Iris5 compability
TARGET_KERNEL_ADDITIONAL_FLAGS := OPLUS_VND_ENV_PW_X5_COMPATIBLE=yes

# Recovery
TARGET_RECOVERY_UI_MARGIN_HEIGHT := 103

# Include the proprietary files BoardConfig.
include vendor/realme/senna/BoardConfigVendor.mk
