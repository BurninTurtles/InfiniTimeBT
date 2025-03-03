#pragma once

#include <lvgl/lvgl.h>
#include <chrono>
#include <cstdint>
#include <memory>
#include <displayapp/Controllers.h>
#include "displayapp/screens/Screen.h"
#include "components/datetime/DateTimeController.h"
#include "components/ble/SimpleWeatherService.h"
#include "utility/DirtyValue.h"
#include "displayapp/apps/Apps.h"

namespace Pinetime {
  namespace Controllers {
    class Settings;
    class Battery;
    class Ble;
    class NotificationManager;
    class MotionController;
  }

  namespace Applications {
    namespace Screens {

      class WatchFaceLCARS : public Screen {
      public:
        WatchFaceLCARS(Controllers::DateTime& dateTimeController,
                          const Controllers::Battery& batteryController,
                          const Controllers::Ble& bleController,
                          Controllers::NotificationManager& notificationManager,
                          Controllers::Settings& settingsController,
                          Controllers::MotionController& motionController,
                          Controllers::HeartRateController& heartRateController,
                          Controllers::SimpleWeatherService& weatherService,
                          Controllers::FS& fs);

        ~WatchFaceLCARS() override;

        void Refresh() override;

        static bool IsAvailable(Pinetime::Controllers::FS& filesystem);

      private:
        uint32_t savedTick = 0;
        uint8_t chargingBatteryPercent = 101; // not a mistake ;)

        uint8_t last_second = 60;

        bool showed_vitals_last = false;

        const char* weekNumberFormat = "%V";

        static constexpr lv_color_t grayColor = LV_COLOR_MAKE(0x99, 0x99, 0x99);
        static constexpr lv_color_t orangeColor = LV_COLOR_MAKE(0xFF, 0x99, 0x33);
        static constexpr lv_color_t redColor = LV_COLOR_MAKE(0xDD, 0x44, 0x44);

        static constexpr lv_color_t bgColor = LV_COLOR_BLACK;
        static constexpr lv_color_t bgBlackColor = LV_COLOR_BLACK;
        static constexpr lv_color_t bgRedColor = LV_COLOR_MAKE(0xe6, 0x44, 0x4a);
        static constexpr lv_color_t bgBlueColor = LV_COLOR_MAKE(0x31, 0x69, 0xd5);
        static constexpr lv_color_t bgLightBlueColor = LV_COLOR_MAKE(0x8b, 0xce, 0xff);
        static constexpr lv_color_t bgVioletColor = LV_COLOR_MAKE(0x9c, 0x44, 0xff);
        static constexpr lv_color_t bgLightVioletColor = LV_COLOR_MAKE(0xd5, 0x99, 0xd5);
        static constexpr lv_color_t bgOrangeColor = LV_COLOR_MAKE(0xff, 0x99, 0x31);
        static constexpr lv_color_t bgYellowColor = LV_COLOR_MAKE(0xff, 0xce, 0x9c);

        Utility::DirtyValue<uint8_t> batteryPercentRemaining {};
        Utility::DirtyValue<bool> isCharging {};
        Utility::DirtyValue<bool> bleState {};
        Utility::DirtyValue<bool> bleRadioEnabled {};
        Utility::DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>> currentDateTime {};
        Utility::DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>> currentNanoSeconds {};
        Utility::DirtyValue<uint32_t> stepCount {};
        Utility::DirtyValue<uint8_t> heartbeat {};
        Utility::DirtyValue<bool> heartbeatRunning {};
        Utility::DirtyValue<std::optional<Pinetime::Controllers::SimpleWeatherService::CurrentWeather>> currentWeather {};
        Utility::DirtyValue<bool> notificationState {};
        using days = std::chrono::duration<int32_t, std::ratio<86400>>; // TODO: days is standard in c++20
        Utility::DirtyValue<std::chrono::time_point<std::chrono::system_clock, days>> currentDate;
        Utility::DirtyValue<size_t> notificationCount {};
        
        lv_obj_t* bg_system_shapes[10];
        lv_obj_t* bg_stardate_shapes[9];
        lv_obj_t* bg_sensors;
        lv_obj_t* bg_vitals_shapes[6];
        lv_obj_t* bg_movement;

        lv_obj_t* bg_label_system;
        lv_obj_t* bg_label_stardate;
        lv_obj_t* bg_label_time;
        lv_obj_t* bg_label_sensors;
        lv_obj_t* bg_label_vitals_or_weather;
        lv_obj_t* bg_label_movement;

        lv_obj_t* timeContainer;
        lv_obj_t* labelTime;
        lv_obj_t* labelTimeAmPm;
        lv_obj_t* label_week;
        lv_obj_t* label_seconds_container;
        lv_obj_t* label_tens_container;
        lv_obj_t* label_ones_container;
        lv_obj_t* label_tens[6];
        lv_obj_t* label_ones[10];
        lv_obj_t* dateContainer;
        lv_obj_t* labelDate;
        lv_obj_t* labelBattery;
        lv_obj_t* system_container;
        lv_obj_t* sensors_container;
        lv_obj_t* bleIcon;
        lv_obj_t* stepIcon;
        lv_obj_t* stepValue;
        lv_obj_t* heartbeatOrWeatherValue;
        lv_obj_t* heartbeatIcon;
        lv_obj_t* weatherIcon;
        lv_obj_t* notificationIcon;

        Controllers::DateTime& dateTimeController;
        const Controllers::Battery& batteryController;
        const Controllers::Ble& bleController;
        Controllers::NotificationManager& notificationManager;
        Controllers::Settings& settingsController;
        Controllers::MotionController& motionController;
        Controllers::HeartRateController& heartRateController;
        Controllers::SimpleWeatherService& weatherService;  

        void SetBatteryLevel(uint8_t batteryPercent, const lv_color_t& color);
        void ResetSecondsDigits();
        void UpdateStepCount();
        void UpdateNotification();
        void UpdateBLE();
        void UpdateBatteryPercent();
        void UpdateStardate();
        void UpdateWK();
        void UpdateSeconds();
        void UpdateTime();
        bool ShowHeartRate();
        void UpdateHeartRate();
        bool ShowWeather();
        void UpdateWeather();

        lv_task_t* taskRefresh;
        lv_font_t* font_antonio_78 = nullptr;
        lv_font_t* font_antonio_33 = nullptr;
        lv_font_t* font_antonio_21 = nullptr;
        lv_font_t* font_antonio_13 = nullptr;
        lv_font_t* font_antonio_12 = nullptr;
      };
    }

    template <>
    struct WatchFaceTraits<WatchFace::LCARS> {
      static constexpr WatchFace watchFace = WatchFace::LCARS;
      static constexpr const char* name = "LCARS face";

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::WatchFaceLCARS(controllers.dateTimeController,
                                              controllers.batteryController,
                                              controllers.bleController,
                                              controllers.notificationManager,
                                              controllers.settingsController,
                                              controllers.motionController,
                                              controllers.heartRateController,
                                              *controllers.weatherController,
                                              controllers.filesystem);
      };

      static bool IsAvailable(Pinetime::Controllers::FS& filesystem) {
        return Screens::WatchFaceLCARS::IsAvailable(filesystem);
      }
    };
  }
}
