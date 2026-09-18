/*
 * Copyright (C) 2021-2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Lights.h"

#include <thread>
#include <chrono>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <fstream>

namespace {

/* clang-format off */
#define LEDS            "/sys/class/leds/backpanel/"
#define RGBCOLOR        LEDS "rgbcolor"
#define MODE            LEDS "mode"
#define TIMINGS         LEDS "timings"
#define BRIGHTNESS      LEDS "brightness"

#define MODE_OFF        0
#define MODE_CC         1
#define MODE_BLINK      2
#define MODE_BREATH     3

#define BATTERY_STATUS_FILE       "/sys/class/power_supply/battery/status"
#define BATTERY_CAPACITY          "/sys/class/power_supply/battery/capacity"
#define BATTERY_FASTCHARGE        "/sys/class/oplus_chg/battery/fast_charge"

#define BATTERY_STATUS_FULL         "Full"
#define BATTERY_STATUS_DISCHARGING  "Discharging"
#define BATTERY_STATUS_CHARGING     "Charging"
/* clang-format on */

enum battery_status {
    BATTERY_UNKNOWN = 0,
    BATTERY_VERY_LOW,
    BATTERY_LOW,
    BATTERY_FREE,
    BATTERY_CHARGING,
    BATTERY_CHARGING_FAST,
    BATTERY_FULL,
};

using ::android::base::WriteStringToFile;

// Write value to path and close file.
bool WriteToFile(const std::string& path, uint32_t content) {
    return WriteStringToFile(std::to_string(content), path);
}

static int get(std::string path) {
    int value;
    std::ifstream file(path);
    
    if (!file.is_open()) {
        LOG(WARNING) << "failed to read from: " << path.c_str();
        return 0;
    }

    file >> value;
    return value;
}

static int readStr(std::string path, char *buffer, size_t size)
{

    std::ifstream file(path);

    if (!file.is_open()) {
        LOG(WARNING) << "failed to read: " << path.c_str();
        return -1;
    }

    file.read(buffer, size);
    file.close();
    return 1;
}

bool getChargerType() {
    return get(BATTERY_FASTCHARGE) ? 1 : 0;
}

int getBatteryStatus()
{
    int err;

    char status_str[16];
    int capacity = 0;

    err = readStr(BATTERY_STATUS_FILE, status_str, sizeof(status_str));
    if (err <= 0) {
        LOG(WARNING) << "failed to read battery status: " << err;
        return BATTERY_UNKNOWN;
    }

    capacity = get(BATTERY_CAPACITY);

    if (0 == strncmp(status_str, BATTERY_STATUS_FULL, 4)) {
            return BATTERY_FULL;
        }

    if (0 == strncmp(status_str, BATTERY_STATUS_DISCHARGING, 11)) {
        if (capacity > 20) {
            return BATTERY_FREE;
        } else if (capacity > 10) {
            return BATTERY_LOW;
        } else {
            return BATTERY_VERY_LOW;
        }
    }

    if (0 == strncmp(status_str, BATTERY_STATUS_CHARGING, 8)) {
        if (capacity <= 80) {
            return getChargerType() ? BATTERY_CHARGING_FAST : BATTERY_CHARGING;
        } else {
            return BATTERY_FULL;
        }
    } else {
        if (capacity < 10) {
            return BATTERY_VERY_LOW;
        } else if (capacity < 20) {
            return BATTERY_LOW;
        } else {
            return BATTERY_FREE;
        }
    }
}

inline int TimingToNum(int timing) {
  LIGHT_LOG("called with arg %d\n", timing);
    if (timing <= 130) {
      return 0;
    } else if (timing <= 260) {
      return 1;
    } else if (timing <= 380) {
      return 2;
    } else if (timing <= 510) {
      return 3;
    } else if (timing <= 770) {
      return 4;
    } else if (timing <= 1040) {
      return 5;
    } else if (timing <= 1600) {
      return 6;
    } else if (timing <= 2100) {
      return 7;
    } else if (timing <= 2600) {
      return 8;
    } else if (timing <= 3100) {
      return 9;
    } else if (timing <= 4200) {
      return 10;
    } else if (timing <= 5200) {
      return 11;
    } else if (timing <= 6200) {
      return 12;
    } else if (timing <= 7300) {
      return 13;
    } else if (timing <= 8300) {
      return 14;
    } else {
      return 15;
    }
}

inline bool IsLit(uint32_t color) {
    return color & 0x00ffffff;
}

}  // anonymous namespace

namespace aidl {
namespace android {
namespace hardware {
namespace light {

#define AutoHwLight(light) \
    { .id = static_cast<int32_t>(light), .ordinal = 0, .type = light }

Lights::Lights() {
        mLights.push_back(AutoHwLight(LightType::BATTERY));
        mLights.push_back(AutoHwLight(LightType::NOTIFICATIONS));
        mLights.push_back(AutoHwLight(LightType::ATTENTION));
        mLights.push_back(AutoHwLight(LightType::MICROPHONE));
        mLights.push_back(AutoHwLight(LightType::CAMERA));

        // Here we start boot animation pattern
        HwLightState state;
        state.color = 0x669900FF;
        state.flashMode = FlashMode::TIMED;
        state.flashOnMs = 1000;
        state.flashOffMs = 1500;
        setLightState(static_cast<int32_t>(LightType::ATTENTION), state);
        LIGHT_LOG("Boot animation started");

        std::thread(&Lights::onBootCompleted, this).detach();
}

void Lights::onBootCompleted() {
    HwLightState state; // Create default clear state
    ::android::base::WaitForProperty("sys.boot_completed", "1");
    LIGHT_LOG("Boot completed, clearing animation");
    setLightState(static_cast<int32_t>(LightType::ATTENTION), state);
}

void Lights::setBatteryRealStatus() {
    mLastRealBatteryState = mLastBatteryState;
    
    switch (getBatteryStatus()) {
        case BATTERY_CHARGING:
            LIGHT_LOG("Battery: Charging");
            mLastRealBatteryState.flashMode = FlashMode::TIMED;
            mLastRealBatteryState.flashOnMs = 1000;
            mLastRealBatteryState.flashOffMs = 1500;
            break;
        case BATTERY_CHARGING_FAST:
            LIGHT_LOG("Battery: Charging Fast");
            mLastRealBatteryState.flashMode = FlashMode::TIMED;
            mLastRealBatteryState.color = 0x10FA10FA;
            mLastRealBatteryState.flashOnMs = 800;
            mLastRealBatteryState.flashOffMs = 1200;
            break;
        case BATTERY_LOW:
            LIGHT_LOG("Battery: Low");
            mLastRealBatteryState.flashMode = FlashMode::TIMED;
            mLastRealBatteryState.flashOnMs = 1000;
            mLastRealBatteryState.flashOffMs = 4500;
            break;
        case BATTERY_VERY_LOW:
            LIGHT_LOG("Battery: Very Low");
            mLastRealBatteryState.flashMode = FlashMode::TIMED;
            mLastRealBatteryState.flashOnMs = 400;
            mLastRealBatteryState.flashOffMs = 6500;
            break;
        case BATTERY_FULL:
            LIGHT_LOG("Battery: Full");
            mLastRealBatteryState.flashMode = FlashMode::NONE;
            break;
        default:
            LIGHT_LOG("Battery: Free");
            break;
    }
    LIGHT_LOG("Final battery status: color=%8x, flashMode=%d, on=%d, off=%d",
              mLastRealBatteryState.color, (int)mLastRealBatteryState.flashMode,
              mLastRealBatteryState.flashOnMs, mLastRealBatteryState.flashOffMs);
        updateNotificationColor();
}

ndk::ScopedAStatus Lights::setLightState(int32_t id, const HwLightState& state) {
    LIGHT_LOG("Call: id=%d, color=%8x, flashMode=%d, on=%d, off=%d",
              id, state.color, (int)state.flashMode, 
              state.flashOnMs, state.flashOffMs);

    LightType type = static_cast<LightType>(id);
    switch (type) {
        case LightType::BATTERY:
            mLastBatteryState = state;
            LIGHT_LOG("Battery");
            setBatteryRealStatus();
            break;
        case LightType::NOTIFICATIONS:
            mLastNotificationsState = state;
            LIGHT_LOG("Notification");
            updateNotificationColor();
            break;
        case LightType::ATTENTION:
            mLastAttentionState = state;
            LIGHT_LOG("Attention");
            updateNotificationColor();
            break;
        case LightType::MICROPHONE:
            mLastMicrophoneState = state;
            updateNotificationColor();
            break;
        case LightType::CAMERA:
            mLastCameraState = state;
            updateNotificationColor();
            break;
        default:
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
            break;
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Lights::getLights(std::vector<HwLight>* _aidl_return) {
    LIGHT_LOG("Call");
    for (const auto& light : mLights) {
        _aidl_return->push_back(light);
    }

    return ndk::ScopedAStatus::ok();
}

//binder_status_t Lights::dump(int fd, const char** /*args*/, uint32_t /*numArgs*/) {
/*    dprintf(fd, "Lights AIDL:\n");
    dprintf(fd, "\n");

    dprintf(fd, "Lights:\n");
    for (const auto& light : mLights) {
        dprintf(fd, "- %d: LightType::%s\n", light.id, toString(light.type).c_str());
    }
    dprintf(fd, "\n");

    dprintf(fd, "Devices:\n");
    mDevices.dump(fd);
    dprintf(fd, "\n");

    return STATUS_OK;
}*/ 

bool Lights::isBatteryDiffer() {
    if (IsLit(mLastBatteryState.color) && 
        (mLastBatteryState.color      != mLastRealBatteryState.color ||
        mLastBatteryState.flashMode   != mLastRealBatteryState.flashMode ||
        mLastBatteryState.flashOnMs   != mLastRealBatteryState.flashOnMs ||
        mLastBatteryState.flashOffMs  != mLastRealBatteryState.flashOffMs)
        ) {
        LIGHT_LOG("Battery state is diffrent");
        return true;
    } else {
        return false;
    }
}

void Lights::updateNotificationColor() {
    char buf[32];
    LIGHT_LOG("Call");
    std::lock_guard<std::mutex> lock(mLedMutex);

    bool isBatteryLit = IsLit(mLastBatteryState.color);
    bool isNotificationsLit = IsLit(mLastNotificationsState.color);
    bool isAttentionLit = IsLit(mLastAttentionState.color);
    bool isMicrophoneLit = IsLit(mLastMicrophoneState.color);
    bool isCameraLit = IsLit(mLastCameraState.color);

    const HwLightState state = isCameraLit          ? mLastCameraState
                               : isMicrophoneLit    ? mLastMicrophoneState
                               : isAttentionLit     ? mLastAttentionState
                               : isNotificationsLit ? mLastNotificationsState
                               : isBatteryDiffer()  ? mLastRealBatteryState
                               : isBatteryLit       ? mLastBatteryState
                                                    : HwLightState();

    if (state.color       == mLastLightState.color &&
        state.flashMode   == mLastLightState.flashMode &&
        state.flashOnMs   == mLastLightState.flashOnMs &&
        state.flashOffMs  == mLastLightState.flashOffMs) {

        LIGHT_LOG("State unchanged, skipping");
        return; // Do nothing, avoid unnecessary flicker since HW chip will reload after any call to mode
    }

    mLastLightState = state;

    if (!IsLit(state.color)) {
        WriteToFile(MODE, MODE_OFF);
        LIGHT_LOG("Not Lit, Turning panel OFF\n");
        return;
    } else {
        /* Write color directly without conversion */
        snprintf(buf, 32, "%8x", state.color);
        WriteStringToFile(buf, RGBCOLOR);
        LIGHT_LOG("written color:%s\n", buf);

        snprintf(buf, 32, "%d", 200);
        WriteStringToFile(buf, BRIGHTNESS);
        LIGHT_LOG("written brightness:%s\n", buf);
        
        if (state.flashMode == FlashMode::NONE) {
            /* Set constant colour if this is requested */
            WriteToFile(MODE, MODE_CC);
            LIGHT_LOG("Color is NOT zero, Mode = NONE turning panel to CC_MODE\n");
        } else if (state.flashMode == FlashMode::TIMED) {
            /* Enable blinking and breathing.*/
            LIGHT_LOG("Color is NOT zero, Mode = TIMED, F_RI:%d F_ON:%d, F_FA:%d F_OFF:%d\n", 0
                      /*state.patternRiseMs*/, state.flashOnMs, 0 /*state.patternFallMs*/, state.flashOffMs);
            /*  Special case for flashOnMs == flashOffMs: use blink effect */
            if (state.flashOnMs == state.flashOffMs) {
                int timing = TimingToNum(state.flashOnMs);
                snprintf(buf, 32, "%#x %#x %#x %#x", 0, timing, 0, timing);
                WriteToFile(MODE, MODE_BLINK);
            } else {
            /*  If flashOnMs != flashOffMs: use breathing effect */
              snprintf(buf, 32, "%#x %#x %#x %#x", 3, TimingToNum(state.flashOnMs), 5, TimingToNum(state.flashOffMs));
              WriteToFile(MODE, MODE_BREATH);
            }
            WriteStringToFile(buf, TIMINGS);
            LIGHT_LOG("written timings:%s\n", buf);
        }
    }

    return;
}

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
