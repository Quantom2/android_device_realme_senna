/*
 * Copyright (C) 2021-2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/light/BnLights.h>
#include <mutex>

#define LOG_TAG "LightService"

#define LIGHT_DEBUG 1

#ifdef LIGHT_DEBUG
#define LIGHT_LOG(fmt, args...)	ALOGW("[%s %d]: " fmt, \
		__func__, __LINE__, ##args)
#else
#define LIGHT_LOG(fmt, args...)
#endif
#define LIGHT_ERR(fmt, args...)	ALOGE("[%s %d]: " fmt, \
		__func__, __LINE__, ##args)

#include <log/log.h>

namespace aidl {
namespace android {
namespace hardware {
namespace light {

class Lights : public BnLights {
  public:
    Lights();

    ndk::ScopedAStatus setLightState(int32_t id, const HwLightState& state) override;
    ndk::ScopedAStatus getLights(std::vector<HwLight>* _aidl_return) override;
/*    binder_status_t dump(int fd, const char** args, uint32_t numArgs) override;*/

  private:
    std::vector<HwLight> mLights;

    HwLightState mLastBatteryState;
    HwLightState mLastRealBatteryState;
    HwLightState mLastNotificationsState;
    HwLightState mLastAttentionState;
    HwLightState mLastMicrophoneState;
    HwLightState mLastCameraState;
    HwLightState mLastLightState;
    std::mutex mLedMutex;

    void onBootCompleted();
    void updateNotificationColor();
    void setBatteryRealStatus();
    bool isBatteryDiffer();
};

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
