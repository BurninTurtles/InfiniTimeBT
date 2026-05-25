#pragma once

#include "displayapp/screens/Screen.h"
#include <lvgl/lvgl.h>

#include "components/stopwatch/StopWatchController.h"
#include "Symbols.h"
#include "utility/DirtyValue.h"

namespace Pinetime::Applications {
  namespace Screens {

    struct TimeSeparated {
      uint16_t hours;
      uint8_t mins;
      uint8_t secs;
      uint8_t hundredths;
      uint32_t epochSecs;
    };

      class StopWatch : public Screen {
      public:
        explicit StopWatch();
        ~StopWatch() override;
        void Refresh() override;

      void PlayPauseBtnEventHandler();
      void StopLapBtnEventHandler();
      bool OnButtonPushed() override;

    private:
      void OnPause();

      void DisplayPaused();
      void DisplayStarted();
      void DisplayCleared();

      void RenderTime();
      void RenderPause();
      void RenderLaps();

      void SetHoursVisible(bool visible);

        States currentState = States::Init;
        TickType_t startTime;
        TickType_t oldTimeElapsed = 0;
        TickType_t blinkTime = 0;
        static constexpr int maxLapCount = 20;
        TickType_t laps[maxLapCount + 1];
        static constexpr int displayedLaps = 2;
        int lapsDone = 0;
        lv_obj_t *time, *msecTime, *btnPlayPause, *btnStopLap, *txtPlayPause, *txtStopLap;
        lv_obj_t* lapText;
        bool isHoursLabelUpdated = false;

      lv_task_t* taskRefresh;
    };
  }

  template <>
  struct AppTraits<Apps::StopWatch> {
    static constexpr Apps app = Apps::StopWatch;
    static constexpr const char* icon = Screens::Symbols::stopWatch;

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::StopWatch();
      };

    static bool IsAvailable(Pinetime::Controllers::FS& /*filesystem*/) {
      return true;
    }
  };
}
