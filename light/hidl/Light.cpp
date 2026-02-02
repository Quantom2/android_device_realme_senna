/*
 * Copyright (C) 2017-2020 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "LightService"

#define LIGHT_DEBUG 1

#ifdef LIGHT_DEBUG
#define LIGHT_LOG(fmt, args...)	ALOGW("[%s] %s %d: " fmt, LOG_TAG, \
		__func__, __LINE__, ##args)
#else
#define LIGHT_LOG(fmt, args...)
#endif
#define LIGHT_ERR(fmt, args...)	ALOGE("[%s] %s %d: " fmt, LOG_TAG, \
		__func__, __LINE__, ##args)

#include <log/log.h>

#include "Light.h"

#include <fstream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LEDS            "/sys/class/leds/backpanel/"
#define RGBCOLOR        LEDS "rgbcolor"
#define MODE            LEDS "mode"
#define TIMINGS         LEDS "timings"

#define MODE_OFF        0
#define MODE_CC         1
#define MODE_BLINK      2
#define MODE_BREATH     3

/*
 * Write value to path and close file.
 */
static void set(std::string path, std::string value) {
    std::ofstream file(path);
    if (!file.is_open()) {
        LIGHT_ERR("failed to write %s to %s\n", value.c_str(), path.c_str());
        return;
    }
    LIGHT_LOG("sucsessfully written %s to %s\n", value.c_str(), path.c_str());
    file << value;
}

static void set(std::string path, int value) {
    set(path, std::to_string(value));
}

static int convert_timing(int timing) {
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

static void set_blink(uint32_t flashOnMs, uint32_t flashOffMs) {
    char buf[32];
    LIGHT_LOG("called with args F_ON:%d, F_OFF:%d\n", flashOnMs, flashOffMs);
    /*  Special case for flashOnMs == flashOffMs: use blink effect */
    if (flashOnMs == flashOffMs) {
        char timing = convert_timing(flashOnMs);
        snprintf(buf, 32, "0x%01x 0x%01x 0x%01x 0x%01x", 0, timing, 0, timing);
        set(MODE, MODE_BLINK);
    } else {
    /*  If flashOnMs != flashOffMs: use breathing effect */
        snprintf(buf, 32, "0x%01x 0x%01x 0x%01x 0x%01x", convert_timing(flashOnMs), 0, convert_timing(flashOffMs), 0);
        set(MODE, MODE_BREATH);
    }
    set(TIMINGS, buf);
    LIGHT_LOG("written timings:%s\n", buf);
}

static void handleNotification(const LightState& state) {
    char buf[32];
    
    /* Turn panel off if COLOR = 0 */
    //if (state.color == 0x00000000) {
    if ((state.color & 0x00FFFFFF ) == 0x000000) {
        set(MODE, MODE_OFF);
        LIGHT_LOG("Color is ZERO, Mode = NONE, Turning panel OFF\n");
        return;
    } else {
        /* Write color directly without conversion */
        snprintf(buf, 32, "0x%02x 0x%02x 0x%02x 0x%02x", (state.color >> 24) & 0xFF, (state.color >> 16) & 0xFF, (state.color >> 8) & 0xFF, state.color & 0xFF);
        set(RGBCOLOR, buf);
        LIGHT_LOG("written color:%s\n", buf);
        
        if (state.flashMode == Flash::NONE) {
            /* Set constant colour if this is requested */
            set(MODE, MODE_CC);
            LIGHT_LOG("Color is NOT zero, Mode = NONE turning panel to CC_MODE\n");
        } else if (state.flashMode == Flash::TIMED) {
            /* Enable blinking and breathing.*/
            LIGHT_LOG("Color is NOT zero, Mode = TIMED, calling set_blink\n");
            set_blink(state.flashOnMs, state.flashOffMs);
        }
    }
}

/* Since higher level code seems to never query which lights
   are actually implemented and keeps trying to set state for backend
   'BUTTONS', drop these requests silently to avoid log spam. */
static void handleIgnore(const LightState&) {
}

static inline bool isLit(const LightState& state) {
    return state.color & 0xFFFFFFFF;
}

static void handleATTENTION(const LightState& state) {
  LIGHT_LOG("ATTENTION CALLED\n");
  handleNotification(state);
}
static void handleNOTIFICATION(const LightState& state) {
  LIGHT_LOG("NOTIFICATION CALLED\n");
  handleNotification(state);
}
static void handleBATTERY(const LightState& state) {
  LIGHT_LOG("BATTERY CALLED\n");
  handleNotification(state);
}
static void handleBUTTONS(const LightState& state) {
  LIGHT_LOG("BUTTONS CALLED, IGNORING\n");
  handleIgnore(state);
}
static void handleBACKLIGHT(const LightState& state) {
  LIGHT_LOG("BACKLIGHT CALLED\n");
  handleNotification(state);
}


/* Keep sorted in the order of importance. */
static std::vector<LightBackend> backends = {
    { Type::ATTENTION, handleATTENTION },
    { Type::NOTIFICATIONS, handleNOTIFICATION },
    { Type::BATTERY, handleBATTERY },
    { Type::BUTTONS, handleBUTTONS },
    { Type::BACKLIGHT, handleBACKLIGHT }
};

namespace android {
namespace hardware {
namespace light {
namespace V2_0 {
namespace implementation {

Return<Status> Light::setLight(Type type, const LightState& state) {
    LightStateHandler handler = nullptr;
    bool handled = false;
    
    /* Lock global mutex until light state is updated. */
    std::lock_guard<std::mutex> lock(globalLock);

    /* Update the cached state value for the current type. */
    for (LightBackend& backend : backends) {
        if (backend.type == type) {
            backend.state = state;
            handler = backend.handler;
            LIGHT_LOG("backend handler chosen");
        }
    }

    /* If no handler has been found, then the type is not supported. */
    if (!handler) {
        LIGHT_ERR("no handler was foud, shutting down");
        return Status::LIGHT_NOT_SUPPORTED;
    }

    /* Light up the type with the highest priority that matches the current handler. */
    for (LightBackend& backend : backends) {
        if (handler == backend.handler && isLit(backend.state)) {
            handler(backend.state);
            handled = true;
            LIGHT_LOG("backend state handler chosen");
            break;
        }
    }

    /* If no type has been lit up, then turn off the hardware. */
    if (!handled) {
        LIGHT_ERR("no type was foud, shutting down");
        handler(state);
    }
    LIGHT_LOG("Sucsesfull setup!");
    return Status::SUCCESS;
}

Return<void> Light::getSupportedTypes(getSupportedTypes_cb _hidl_cb) {
    std::vector<Type> types;

    for (const LightBackend& backend : backends) {
        types.push_back(backend.type);
    }
    
    _hidl_cb(types);

    return Void();
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace light
}  // namespace hardware
}  // namespace android
